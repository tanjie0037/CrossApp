package com.zpt.libjpush;

import org.CrossApp.lib.Cocos2dxHelper;
import org.json.JSONException;
import org.json.JSONObject;

import com.zpt.utils.ZPTLog;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
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

		ZPTLog.d("bundle:" + bundle.toString());

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

			try {
				JSONObject obj = new JSONObject(bundleStr);
				// 打开url的直接跳转
				if (obj != null &&
						obj.getString("msg_type") != null && obj.getString("msg_type").equals("message") &&
						obj.getString("url") != null && !obj.getString("url").equals("")) {

					Intent intentUrl = new Intent(Intent.ACTION_VIEW, Uri.parse(obj.getString("url")));
					intentUrl = Intent.createChooser(intentUrl, "Open With");
					intentUrl.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
					context.startActivity(intentUrl);

				} else {
					ComponentName componetName = new ComponentName(
							"com.zpt.appgift",
							"com.zpt.appgift.AppGift");
					Intent startIntent= new Intent();
					startIntent.putExtra("JPushMsg", bundleStr);
					startIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
					startIntent.setComponent(componetName);
					context.startActivity(startIntent);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

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

		String extras = bundle.getString(JPushInterface.EXTRA_EXTRA);
		JSONObject extraJson;

		try {
			if (extras == null || extras.equals("")) {
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
		
		return infoStr;
	}
}
