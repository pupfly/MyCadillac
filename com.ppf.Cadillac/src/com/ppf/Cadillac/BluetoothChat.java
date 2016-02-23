/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.ppf.Cadillac;

import java.util.Calendar;
import java.util.TimeZone;

import com.example.android.BluetoothChat.R;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import android.util.Log;
import android.view.KeyEvent;

import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.RelativeLayout;

import android.widget.TextView;
import android.widget.Toast;
import android.hardware.SensorManager;
import android.hardware.Sensor;
import android.hardware.SensorEventListener;
import android.hardware.SensorEvent;
/**
 * This is the main Activity that displays the current chat session.
 */
@SuppressLint("NewApi")
public class BluetoothChat extends Activity implements SensorEventListener {
    // Debugging
    private static final String TAG = "BluetoothChat";
    private static final boolean D = true;
    //about sensor 
    private SensorManager mSensorManager;
    private Sensor mOrientationSensor;
    private static boolean should_stop = true, is_fine_turning = false;
    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int MESSAGE_DEVICE_NAME_CONNECTING = 6;
 
    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    private static final int REQUEST_ENABLE_BT = 3;
    
    private static final int TARGET_AT_CAR = 0;
    private static final int TARGET_AT_FDJ = 1;
    private static final int TARGET_AT_BDJ = 2;
    //buffer for measure data transfer
    private static StringBuilder measure_buf = new StringBuilder();
    //command codes 
    private static String[] command_up = {"U", "f"};
    private static String[] command_left = {"L", "l", "1", "L", "-", "J"};
    private static String[] command_right = {"R", "r", "3", "R", "+", "j"};
    private static String[] command_down = {"D",  "",  "2"};
    private static String command_stop = "s";
    // Layout Views
    private RelativeLayout layoutMain, layoutControl;
    private TextView mTitle,mTemperature,mHumidity,mFDistance,mBDistance,mTextPrompt;
    private ListView mConversationView;
    private Button mbutton_left;
    private Button mbutton_up;
    private Button mbutton_down;
    private Button mbutton_stop;
    private Button mbutton_right;
    private Button mbutton_send;
    private Button mbutton_clear;
    private Button mbutton_enable;
    private Button mbutton_enable_log;
    private Button mbutton_enable_sensor;
    private Button mbutton_start_control;
    private Button mbutton_level1;
    private Button mbutton_level2;
    private Button mbutton_level3;
    private Button mbutton_level4;
    private Button mbutton_level5;
    private Button mbutton_return;
    private Button mbutton_fine_turning;
    private Button mbutton_renew_ht;
    private Button mbutton_renew_fb;
    private Button mbutton_renew_auto;
    private Button mbutton_auto_mode;
    private EditText mEdit_msg;
    private RadioButton target_car, target_fdj, target_bdj;
    private static int target = TARGET_AT_CAR;
    // Name of the connected device
    private String mConnectedDeviceName = null;
    private String mConnectingDevice = null;
    // Array adapter for the conversation thread
    private ArrayAdapter<String> mConversationArrayAdapter;
    // String buffer for outgoing messages
    private StringBuffer mOutStringBuffer;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
    private BluetoothChatService mChatService = null;
    
    int cout=0;
    public float abs(float v)
    {
    	return v >= 0 ? v : -v;
    }
    public float max(float x, float y)
    {
    	return x >= y ? x : y;
    }
    public void manage_measure_data(String msg)
    {
    	try{
    		if(msg.length() == 0)
    		{
    			return;
    		}
    		char type = msg.charAt(0);
    		switch(type)
    		{
    		case 'H':
    			mHumidity.setText("湿度(%)：" + msg.toString().substring(1));
    			break;
    		case 'T':
    			mTemperature.setText("温度(℃)：" + msg.substring(1));
    			break;
    		case 'D':
    			mFDistance.setText("前距(cm)：" + msg.substring(1));
    			break;
    		case 'd':
    			mBDistance.setText("后距(cm)：" + msg.substring(1));
    			break;
    		case 'M':
    			mConversationArrayAdapter.add("消息：" + msg.substring(1));
    			break;
    		default:
    			mConversationArrayAdapter.add("误码：" + msg.substring(1));
    			break;
    		}
    	}
    	catch(Exception e)
    	{
    		Toast.makeText(getApplicationContext(), "通信出错！", Toast.LENGTH_SHORT).show();
    	}
    }
    @SuppressLint("NewApi")
	@Override
    public void onSensorChanged(SensorEvent event)
    {
    	try{
    		float fb = event.values[1];
        	float lr = event.values[2];
        	if (max(abs(lr),abs(fb)) > 15)//sensitivity
        	{
        		should_stop = true;
        		if((abs(lr) > abs(fb)))//turn left/right
        		{
        			if(lr >= 0)//turn left
            		{
        				mbutton_left.setPressed(true);
        				sendMessage(command_left[target]);
        				mbutton_left.setPressed(false);
            		}
            		else				 //turn right
            		{
            			mbutton_right.setPressed(true);
            			sendMessage(command_right[target]);
            			mbutton_right.setPressed(false);
            		}
        		}
        		else	//back/forward command
        		{
        			if(fb >= 0)//forward
            		{
        				mbutton_up.setPressed(true);
        				sendMessage(command_up[target]);
        				mbutton_up.setPressed(false);
            		}
            		else				 //backward
            		{
            			if(mbutton_down.isEnabled())
            			{
            				mbutton_down.setPressed(true);
            				sendMessage(command_down[target]);
            				mbutton_down.setPressed(false);
            			}
            		}
        		}
        	}
        	else
        	{
        		if(should_stop)//send stop command
    			{
    				mbutton_stop.setPressed(true);
        			mbutton_stop.performClick();
        			mbutton_stop.setPressed(false);
        			should_stop = false;
    			}
        	}
    	}
    	catch(Exception e)
    	{
    		Toast.makeText(getApplicationContext(), e.getMessage() + "请关闭传感器！", Toast.LENGTH_SHORT).show();
    	}
    }

    @SuppressLint("NewApi")
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");
        //sensor setup
        mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        mOrientationSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
        // Set up the window layout
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.main);
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);

        // Set up the custom title
        mTitle = (TextView) findViewById(R.id.title_left_text);
        mTitle.setText(R.string.app_name);
        mTitle = (TextView) findViewById(R.id.title_right_text);

        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        
        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
    }

    @SuppressLint("NewApi")
	@Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");

        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        // Otherwise, setup the chat session
        } else {
            if (mChatService == null) setupChat();
        }
        try{
        	mEdit_msg.clearFocus();
        }
        catch(Exception e){
        	
        }
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");
        try{
        	if(mbutton_enable_sensor.getText().toString().equals("关传感器"))
            {
            	mSensorManager.registerListener(BluetoothChat.this, mOrientationSensor,SensorManager.SENSOR_DELAY_NORMAL);
            }
        }
        catch(Exception e)
        {
        	
        }
        
        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
              // Start the Bluetooth chat services
              mChatService.start();
            }
        }
    }

    @SuppressLint("NewApi")
	private void setupChat() {
    	try{
            Log.d(TAG, "setupChat()");
            //Initialize sensor
            
            // Initialize the array adapter for the conversation thread
            mConversationArrayAdapter = new ArrayAdapter<String>(this, R.layout.message);
            mConversationView = (ListView) findViewById(R.id.in);
            mConversationView.setAdapter(mConversationArrayAdapter);
            //Initialize the EditText
            mEdit_msg = (EditText)findViewById(R.id.edit_msg);
            //Initialize the texts
            mTemperature = (TextView)findViewById(R.id.text_temperature);
            mHumidity = (TextView)findViewById(R.id.text_humidity);
            mFDistance = (TextView)findViewById(R.id.text_f_distance);
            mBDistance = (TextView)findViewById(R.id.text_b_distance);
            mTextPrompt = (TextView)findViewById(R.id.text_prompt);
            //Initialize the layout
            layoutMain = (RelativeLayout)findViewById(R.id.RelativeLayoutMain);
            layoutControl = (RelativeLayout)findViewById(R.id.layout_control);
            // Initialize the compose field with a listener for the return key
            mbutton_left = (Button) findViewById(R.id.button_left);
            mbutton_up = (Button) findViewById(R.id.button_up);
            mbutton_down = (Button) findViewById(R.id.button_down);
            mbutton_stop = (Button) findViewById(R.id.button_stop);
            mbutton_send = (Button) findViewById(R.id.button_send);
            mbutton_clear = (Button) findViewById(R.id.button_clear);
            mbutton_right = (Button) findViewById(R.id.button_right);
            mbutton_enable = (Button) findViewById(R.id.button_enable);
            mbutton_enable_log = (Button) findViewById(R.id.button_show_log);
            mbutton_enable_sensor = (Button) findViewById(R.id.button_enable_sensor);
            mbutton_start_control = (Button) findViewById(R.id.button_start_control);
            mbutton_level1 = (Button) findViewById(R.id.button_level1);
            mbutton_level2 = (Button) findViewById(R.id.button_level2);
            mbutton_level3 = (Button) findViewById(R.id.button_level3);
            mbutton_level4 = (Button) findViewById(R.id.button_level4);
            mbutton_level5 = (Button) findViewById(R.id.button_level5);
            mbutton_return = (Button) findViewById(R.id.button_return);
            mbutton_fine_turning = (Button) findViewById(R.id.button_fine_turning);
            mbutton_renew_ht = (Button) findViewById(R.id.button_DHT11);
            mbutton_renew_fb = (Button) findViewById(R.id.button_distance);
            mbutton_renew_auto = (Button) findViewById(R.id.button_auto);
            mbutton_auto_mode =  (Button) findViewById(R.id.button_auto_mode);
            target_car = (RadioButton)findViewById(R.id.radio_car);
            target_fdj = (RadioButton)findViewById(R.id.radio_fdj);
            target_bdj = (RadioButton)findViewById(R.id.radio_bdj);
            
            target_car.setOnClickListener(new OnClickListener(){
    			@SuppressLint("NewApi")
    			@Override
    			public void onClick(View v)
    			{
    				mbutton_down.setEnabled(true);
    				mbutton_up.setEnabled(true);
    				target = TARGET_AT_CAR;
    				mbutton_stop.performClick();
    				Toast.makeText(getApplicationContext(), "Target at Engines!", Toast.LENGTH_SHORT).show();
    			}
    		});
            
            target_fdj.setOnClickListener(new OnClickListener(){
    			@SuppressLint("NewApi")
    			@Override
    			public void onClick(View v)
    			{
    				target = TARGET_AT_FDJ;
    				mbutton_up.setEnabled(true);
    				mbutton_down.setEnabled(false);
    				mbutton_stop.performClick();
    				Toast.makeText(getApplicationContext(), "Target at Front Steering Engine!", Toast.LENGTH_SHORT).show();
    			}
    		});
            
            target_bdj.setOnClickListener(new OnClickListener(){
    			@SuppressLint("NewApi")
    			@Override
    			public void onClick(View v)
    			{
    				target = TARGET_AT_BDJ;
    				mbutton_down.setEnabled(true);
    				mbutton_up.setEnabled(false);
    				mbutton_stop.performClick();
    				Toast.makeText(getApplicationContext(), "Target at Back Steering Engine!", Toast.LENGTH_SHORT).show();
    			}
    		});
            //show_log button
            mbutton_enable_log.setOnClickListener(new OnClickListener() {
            	private int mark = 1;
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	if(mark==0){
                		mTextPrompt.setVisibility(EditText.VISIBLE);
                		mConversationView.setVisibility(Button.VISIBLE);
                		if(mEdit_msg.getVisibility() != EditText.VISIBLE)//if button clear is needed
                			mbutton_clear.setVisibility(Button.VISIBLE);
                		mbutton_enable_log.setText("关闭日志");
                		mark=1;
                	}
                	else{
                		mTextPrompt.setVisibility(EditText.INVISIBLE);
                		mConversationView.setVisibility(Button.INVISIBLE);
                		if(mEdit_msg.getVisibility() == EditText.VISIBLE)//if button clear is needed
                			mbutton_clear.setVisibility(Button.VISIBLE);
                		else
                			mbutton_clear.setVisibility(Button.INVISIBLE);
                		mbutton_enable_log.setText("显示日志");
                		mark=0;
                	}
                   
                }
            });
            //enable button
            mbutton_enable.setOnClickListener(new OnClickListener() {
            	private int mark = 0;
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	if(mark==0){
                		 mEdit_msg.setVisibility(EditText.VISIBLE);
                		 mbutton_send.setVisibility(Button.VISIBLE);
                		 if(mTextPrompt.getVisibility() != TextView.VISIBLE)//if button clear is  never needed
                			 mbutton_clear.setVisibility(Button.VISIBLE);
                		 mbutton_enable.setText("关闭调试");
                		mark=1;
                	}
                	else{
                		 mEdit_msg.setVisibility(EditText.INVISIBLE);
                		 mbutton_send.setVisibility(Button.INVISIBLE);
                		 if(mTextPrompt.getVisibility() == TextView.VISIBLE)//if button clear is  never needed
                			mbutton_clear.setVisibility(Button.VISIBLE);
                		 else
                 			mbutton_clear.setVisibility(Button.INVISIBLE);
                		 mbutton_enable.setText("启动调试");
                		mark=0;
                	}
                   
                }
            });
            //enable sensor button
            mbutton_enable_sensor.setOnClickListener(new OnClickListener() {
            	private int mark = 0;
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	try{
                		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
                            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
                            return;
                        }
                		if(mark==0){
                    		mSensorManager.registerListener(BluetoothChat.this, mOrientationSensor,SensorManager.SENSOR_DELAY_NORMAL);
                    		mbutton_enable_sensor.setText("关传感器");
                    		mTextPrompt.setText("运行日志(方向传感器工作中)：");
                    		mark=1;
                    	}
                    	else{
                    		mbutton_stop.performClick();
                    		mSensorManager.unregisterListener(BluetoothChat.this);
                    		mbutton_enable_sensor.setText("开传感器");
                    		mTextPrompt.setText("运行日志：");
                    		
                    		mark=0;
                    	}
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), "传感器设置失败！", Toast.LENGTH_SHORT);
                	}
                }
            });
            //right  button
            mbutton_right.setOnTouchListener(new OnTouchListener() {

    			@Override
    			public boolean onTouch(View v, MotionEvent event) {
    				int action = event.getAction();

    				switch (action)
    				{
    				case MotionEvent.ACTION_DOWN:
    					if(is_fine_turning)
    					{
    						sendMessage(command_right[target + 3]);
    					}
    					else
    						sendMessage(command_right[target]);
    					break;
    				case MotionEvent.ACTION_UP:
    					sendMessage("s");
    					break;
    				}
    				return false;
    			}
    			});
            //left button
            mbutton_left.setOnTouchListener(new OnTouchListener() {

    			@Override
    			public boolean onTouch(View v, MotionEvent event) {
    				int action = event.getAction();

    				switch (action)
    				{
    				case MotionEvent.ACTION_DOWN:
    					if(is_fine_turning)
    					{
    						sendMessage(command_left[target + 3]);
    					}
    					else
    						sendMessage(command_left[target]);
    					break;
    				case MotionEvent.ACTION_UP:
    					sendMessage("s");
    					break;
    				}
    				return false;
    			}
    			});
            //up
            mbutton_up.setOnTouchListener(new OnTouchListener() {

    			@Override
    			public boolean onTouch(View v, MotionEvent event) {
    				int action = event.getAction();

    				switch (action)
    				{
    				case MotionEvent.ACTION_DOWN:
    					sendMessage(command_up[target]);
    					break;
    				case MotionEvent.ACTION_UP:
    					sendMessage("s");
    					break;
    				}
    				return false;
    			}
    			});
          //down
            mbutton_down.setOnTouchListener(new OnTouchListener() {

    			@Override
    			public boolean onTouch(View v, MotionEvent event) {
    				int action = event.getAction();

    				switch (action)
    				{
    				case MotionEvent.ACTION_DOWN:
    					if(target_bdj.isChecked())
    						sendMessage("2");
    					else
    						sendMessage("D");
    					break;
    				case MotionEvent.ACTION_UP:
    					sendMessage("s");
    					break;
    				}
    				return false;
    			}
    			});
          //stop
            mbutton_stop.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	sendMessage(command_stop);               
                }
            });
            //Initialize clear button
            mbutton_clear.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	try{
                		mConversationArrayAdapter.clear();
                		mEdit_msg.setText("");
                		Toast.makeText(getApplicationContext(),"已清空！", Toast.LENGTH_SHORT).show();
                	}   
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), "清空对话内容失败！", Toast.LENGTH_SHORT).show();
                	}
                }
            });
            //Initialize send button
            mbutton_send.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    // Send a message using content of the edit text widget
                	try{
                		String message = mEdit_msg.getText().toString();
                    	//if you want to set new command key
                    	if(message.isEmpty())
                    		return;
                    	char ch = message.charAt(0);
                    	if(ch == '$')
                    	{
                    		if(message.length() >= 5)
                    		{
                    			try{
                        			switch(message.charAt(1))
                        			{
                        			case 'U':
                        				command_up[0] = (message.substring(2, 3));
                        				command_up[1] = (message.substring(3, 4));
                        				Toast.makeText(getApplicationContext(), "↑命令键值已更新！", Toast.LENGTH_SHORT).show();
                        				break;
                        			case 'D':
                        				command_down[0] = (message.substring(2, 3));
                						command_down[2] = (message.substring(3, 4));
                						Toast.makeText(getApplicationContext(), "↓命令键值已更新！", Toast.LENGTH_SHORT).show();
                        				break;
                        			case 'L':
                        				command_left[0] = (message.substring(2, 3));
                        				command_left[1] = (message.substring(3, 4));
                        				command_left[2] = (message.substring(4, 5));
                        				command_left[3] = (message.substring(5, 6));
                        				command_left[4] = (message.substring(6, 7));
                        				command_left[5] = (message.substring(7, 8));
                        				Toast.makeText(getApplicationContext(), "←命令键值已更新！", Toast.LENGTH_SHORT).show();
                        				break;
                        			case 'R':
                        				command_right[0] = (message.substring(2, 3));
                        				command_right[1] = (message.substring(3, 4));
                        				command_right[2] = (message.substring(4, 5));
                        				command_right[3] = (message.substring(5, 6));
                        				command_right[4] = (message.substring(6, 7));
                        				command_right[5] = (message.substring(7, 8));
                        				Toast.makeText(getApplicationContext(), "→命令键值已更新！", Toast.LENGTH_SHORT).show();
                        				break;
                        			}
                        		}
                        		catch(Exception e)
                        		{
                        			Toast.makeText(getApplicationContext(), "命令解析错误！", Toast.LENGTH_SHORT).show();
                        		}
                    		}
                    		else
                    		{
                    			if(message.length() >= 3 && message.charAt(1) == 'S')
                    			{
                    				try{
                    					command_stop = (message.substring(2, 3));
            							Toast.makeText(getApplicationContext(), "S命令键值已更新！", Toast.LENGTH_SHORT).show();
                    				}
                    				catch(Exception e)
                    				{
                    					Toast.makeText(getApplicationContext(), "命令解析错误！", Toast.LENGTH_SHORT).show();
                    				}
                    			}
                    		}
                    		return;
                    	}
                    	sendMessage(message);
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}               
                }
            });
            //Initialize the start control button
            mbutton_start_control.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
                            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
                            return;
                        }
                		layoutMain.setVisibility(RelativeLayout.INVISIBLE);
                		layoutControl.setVisibility(RelativeLayout.VISIBLE);
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            //Initialize the return button
            mbutton_return.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		layoutMain.setVisibility(RelativeLayout.VISIBLE);
                		layoutControl.setVisibility(RelativeLayout.INVISIBLE);
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
          //Initialize the levels button
            mbutton_level1.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		sendMessage("A");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            mbutton_level2.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		sendMessage("B");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            mbutton_level3.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		sendMessage("C");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            mbutton_level4.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		sendMessage("E");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            mbutton_level5.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		sendMessage("F");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            //Initialize button fine turning
            mbutton_fine_turning.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                	try{
                		if(is_fine_turning)
                		{
                			is_fine_turning = false;
                			mbutton_fine_turning.setText("舵机微调");
                			return;
                		}
                		is_fine_turning = true;
            			mbutton_fine_turning.setText("关闭微调");
            			return;
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
                	}
                });
            //Initialize button renew DHT11
            mbutton_renew_ht.setOnClickListener(new OnClickListener(){

				@Override
				public void onClick(View arg0) {
					// TODO Auto-generated method stub
					try{
                		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
                            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
                            return;
                        }
                		sendMessage("n");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
				}});
            mbutton_renew_fb.setOnClickListener(new OnClickListener(){

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					try{
                		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
                            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
                            return;
                        }
                		sendMessage("N");
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
				}});
            //initialize button auto
            mbutton_renew_auto.setOnClickListener(new OnClickListener(){
            	private int mark = 0;
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					try{
                		if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
                            Toast.makeText(getApplicationContext(), R.string.not_connected, Toast.LENGTH_SHORT).show();
                            return;
                        }
                		if(mark == 0)
                		{
                			sendMessage("#");
                			mark = 1;
                			mbutton_renew_auto.setText("自动更新");
                		}
                		else
                		{
                			sendMessage("$");
                			mark = 0;
                			mbutton_renew_auto.setText("手动更新");
                		}
                	}
                	catch(Exception e)
                	{
                		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                	}
				}});
            //Initialize button auto mode
            mbutton_auto_mode.setOnClickListener(new OnClickListener(){
            	private int mark = 0;
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					if(mark == 0)
            		{
            			sendMessage("P");
            			mark = 1;
            			mbutton_auto_mode.setText("自动运行");
            		}
            		else
            		{
            			sendMessage("s");
            			mark = 0;
            			mbutton_auto_mode.setText("手动控制");
            		}
				}});
            // Initialize the BluetoothChatService to perform bluetooth connections
            mChatService = new BluetoothChatService(this, mHandler);

            // Initialize the buffer for outgoing messages
            mOutStringBuffer = new StringBuffer("");
    	}
    	catch(Exception e)
    	{
    		Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
    	}
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
        try{
        	mSensorManager.unregisterListener(BluetoothChat.this);
        }
        catch(Exception e)
        {
        	
        }
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        super.onStop();
        try{
        	mSensorManager.unregisterListener(BluetoothChat.this);
        }
        catch(Exception e)
        {
        	
        }
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        try{
        	mSensorManager.unregisterListener(BluetoothChat.this);
        }
        catch(Exception e)
        {
        	
        }
        
        // Stop the Bluetooth chat services
        if (mChatService != null) mChatService.stop();
        if(D) Log.e(TAG, "--- ON DESTROY ---");
    }

    @SuppressLint("NewApi")
	private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        if (mBluetoothAdapter.getScanMode() !=
            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }

    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);

            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
        }
    }

    // The action listener for the EditText widget, to listen for the return key
    @SuppressLint({ "NewApi", "HandlerLeak" })
	private TextView.OnEditorActionListener mWriteListener =
        new TextView.OnEditorActionListener() {
        public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
            // If the action is a key-up event on the return key, send the message
            if (actionId == EditorInfo.IME_NULL && event.getAction() == KeyEvent.ACTION_UP) {
                String message = view.getText().toString();
                sendMessage(message);
            }
            if(D) Log.i(TAG, "END onEditorAction");
            return true;
        }
    };

    // The Handler that gets information back from the BluetoothChatService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
        	Calendar now = Calendar.getInstance();
		    TimeZone timeZone = now.getTimeZone();
			long totalMilliseconds = System.currentTimeMillis() + timeZone.getRawOffset();
			long totalSeconds = totalMilliseconds / 1000;
			int currentSecond = (int)(totalSeconds % 60);
			long totalMinutes = totalSeconds / 60;
			int currentMinute = (int)(totalMinutes % 60);
			long totalHours = totalMinutes / 60;
			int currentHour = (int)(totalHours % 24);
            switch (msg.what) {
            
            case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case BluetoothChatService.STATE_CONNECTED:
                    mTitle.setText(R.string.title_connected_to);
                    mTitle.append(mConnectedDeviceName);
                    mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"： " + "已连接：" + mConnectedDeviceName);
                    BluetoothChat.this.sendMessage("S");
                    break;
                case BluetoothChatService.STATE_CONNECTING:
                    mTitle.setText(R.string.title_connecting);
                    mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"： " + "尝试连接：" + mConnectingDevice);
                    break;
                case BluetoothChatService.STATE_LISTEN:
                case BluetoothChatService.STATE_NONE:
                    mTitle.setText(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                // construct a string from the buffer
                String writeMessage = new String(writeBuf);
                mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"： " + "发送命令:" + writeMessage);
                break;
            case MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
				String readMessage = new String(readBuf, 0, msg.arg1);
				char ch=readMessage.charAt(0);
				if(ch == '$')//一条完整的消息传送开始的标志
				{//发送确认消息
					BluetoothChat.this.sendMessage("&");
					mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"： " + "Uploading data --from MCU..." );
				}
				else if(ch == '#')//一条完整的消息传送完毕的标志
				{
					BluetoothChat.this.sendMessage("&");//发送确认消息
					if(measure_buf.length() >= 1)
					{
						manage_measure_data(measure_buf.toString());
					}
					else
					{
						mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"：意外的终止符第 " + ++cout + " 次" );
					}
					measure_buf = new StringBuilder();
				}
				else{
					BluetoothChat.this.sendMessage("&");//发送确认消息
					measure_buf.append(readMessage);//收集消息
					mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond+"： " + readMessage + "--from MCU..." );
				}
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_DEVICE_NAME_CONNECTING:
            	mConnectingDevice = msg.getData().getString(DEVICE_NAME);
            	break;
            case MESSAGE_TOAST:
            	Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST), Toast.LENGTH_SHORT).show();
                mConversationArrayAdapter.add(currentHour + ":" + currentMinute + ":" + currentSecond + "： " + msg.getData().getString(TOAST) + "--from server..." );
                break;
            }
        }
    };

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE_SECURE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                connectDevice(data, true);
            }
            break;
        case REQUEST_CONNECT_DEVICE_INSECURE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                connectDevice(data, false);
            }
            break;
        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupChat();
            } else {
                // User did not enable Bluetooth or an error occured
                Log.d(TAG, "BT not enabled");
                Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    @SuppressLint({ "NewApi", "NewApi" })
	private void connectDevice(Intent data, boolean secure) {
        // Get the device MAC address
        String address = data.getExtras()
            .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        // Get the BLuetoothDevice object
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        // Attempt to connect to the device
        mChatService.connect(device, secure);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Intent serverIntent = null;
        switch (item.getItemId()) {
        case R.id.secure_connect_scan:
        	// If BT is not on, request that it be enabled.
            // setupChat() will then be called during onActivityResult
            if (!mBluetoothAdapter.isEnabled()) {
                Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            // Otherwise, setup the chat session
            } else {
                if (mChatService == null) setupChat();
            }
            // Launch the DeviceListActivity to see devices and do scan
            serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
            return true;
        case R.id.discoverable:
            // Ensure this device is discoverable by others
            ensureDiscoverable();
            return true;
        }
        return false;
    }

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

}
