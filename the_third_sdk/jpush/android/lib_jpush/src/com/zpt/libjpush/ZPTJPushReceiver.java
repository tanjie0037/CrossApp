package com.zpt.libjpush;

import org.CrossApp.lib.Cocos2dxHelper;
import org.json.JSONException;
import org.json.JSONObject;

import com.zpt.utils.ZPTLog;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import cn.jpush.android.api.JPushInterface;

/**
 * 自定义接收器
 * 
 * 如果不定义这个 Receiver，则： 1) 默认用户会打开主界面 2) 接收不到自定义消息
 */
public class ZPTJPushReceiver extends BroadcastReceiver {
	private static final String TAG = "JPush";
	private static Class<?> _cls = null;

	public static void init(Class<?> cls) {
		_cls = cls;
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		Bundle bundle = intent.getExtras();
		String bundleStr = toNativeMessage(context, bundle);

		ZPTLog.v("[MyReceiver] onReceive - " + intent.getAction() + ", info: " + bundleStr);

		if (JPushInterface.ACTION_REGISTRATION_ID.equals(intent.getAction())) {
			String regId = bundle.getString(JPushInterface.EXTRA_REGISTRATION_ID);
			ZPTLog.v("[MyReceiver] 接收Registration Id : " + regId);
			// send the Registration Id to your server...

		} else if (JPushInterface.ACTION_MESSAGE_RECEIVED.equals(intent.getAction())) {
			ZPTLog.v("[MyReceiver] 接收到推送下来的自定义消息: " + bundle.getString(JPushInterface.EXTRA_MESSAGE));

			// 转发到native
			Cocos2dxHelper.postNotification(Cocos2dxHelper.kNotiRemoteNotiRecived, bundleStr);

		} else if (JPushInterface.ACTION_NOTIFICATION_RECEIVED.equals(intent.getAction())) {
			ZPTLog.v("[MyReceiver] 接收到推送下来的通知");
			int notifactionId = bundle.getInt(JPushInterface.EXTRA_NOTIFICATION_ID);
			ZPTLog.v("[MyReceiver] 接收到推送下来的通知的ID: " + notifactionId);

			// 转发到native
			if (_cls != null) {
				Cocos2dxHelper.postNotification(Cocos2dxHelper.kNotiRemoteNotiRecived, bundleStr);
			}

		} else if (JPushInterface.ACTION_NOTIFICATION_OPENED.equals(intent.getAction())) {
			ZPTLog.v("[MyReceiver] 用户点击打开了通知");

			// 打开自定义的Activity
			if (_cls == null) {
				assert(false);
				return;
			}

			Intent i = new Intent(context, _cls);
			i.putExtras(bundle);
			// i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
			context.startActivity(i);

		} else if (JPushInterface.ACTION_RICHPUSH_CALLBACK.equals(intent.getAction())) {
			ZPTLog.v("[MyReceiver] 用户收到到RICH PUSH CALLBACK: " + bundle.getString(JPushInterface.EXTRA_EXTRA));
			// 在这里根据 JPushInterface.EXTRA_EXTRA 的内容处理代码，比如打开新的Activity，
			// 打开一个网页等..

		} else if (JPushInterface.ACTION_CONNECTION_CHANGE.equals(intent.getAction())) {
			boolean connected = intent.getBooleanExtra(JPushInterface.EXTRA_CONNECTION_CHANGE, false);
			ZPTLog.v(TAG, "[MyReceiver]" + intent.getAction() + " connected state change to " + connected);

		} else {
			ZPTLog.v("[MyReceiver] Unhandled intent - " + intent.getAction());
		}
	}

	// send msg to MainActivity
	private String toNativeMessage(Context context, Bundle bundle) {
		String infoStr = "";
		
		if (_cls != null) {
			String extras = bundle.getString(JPushInterface.EXTRA_EXTRA);
			JSONObject extraJson;

			try {
				if (extras == null) {
					extraJson = new JSONObject();
				} else {
					extraJson = new JSONObject(extras);					
				}

				for (String key : bundle.keySet()) {
				    if (key.equals(JPushInterface.EXTRA_EXTRA)) {
				    	continue;
				    } else if (key.equals(JPushInterface.EXTRA_NOTIFICATION_ID)) {
				    	extraJson.put(key, bundle.getInt(key));
					} else if (key.equals(JPushInterface.EXTRA_CONNECTION_CHANGE)) {
						extraJson.put(key, bundle.getBoolean(key));
					} else {
						extraJson.put(key, bundle.getString(key));
					}
				}
				
				infoStr = extraJson.toString();
				
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return infoStr;
	}
}
