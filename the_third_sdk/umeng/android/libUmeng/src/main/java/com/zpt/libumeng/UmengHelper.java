package com.zpt.libumeng;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.CrossApp.lib.CrossAppActivity;
import org.CrossApp.lib.CrossAppHelper;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import com.umeng.analytics.AnalyticsConfig;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.onlineconfig.UmengOnlineConfigureListener;
import com.umeng.fb.FeedbackAgent;
import com.umeng.fb.model.UserInfo;

public class UmengHelper {
	private static boolean _init = false;
	private static CrossAppActivity _context = null;

	public static void initWithContex(CrossAppActivity ctx) {
		_context = ctx;
	}

	static void startWithAppkey(String appkey, String channel) {
		if (null == _context) {
			assert (false);
			return;
		}

		AnalyticsConfig.setAppkey(appkey);
		AnalyticsConfig.setChannel(channel);

		MobclickAgent.setOnlineConfigureListener(new UmengOnlineConfigureListener() {
			@Override
			public void onDataReceived(JSONObject data) {
				CrossAppHelper.postNotification(CrossAppHelper.kOnlineParamUpdated);
			}
		});
		
		MobclickAgent.updateOnlineConfig(_context);

		// 没办法友盟只有参数变了才能回调
		new Thread(new Runnable() {
			public void run() {
				try {
					Thread.sleep(3000);
					CrossAppHelper.postNotification(CrossAppHelper.kOnlineParamUpdated);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}).start();

		// FeedbackAgent agent = new FeedbackAgent(_context);
		// agent.sync();

		_init = true;
	}

	static void onPageStart(String ViewID) {
		MobclickAgent.onPageStart(ViewID);
		onResume();
	}

	static void onPageEnd(String ViewID) {
		MobclickAgent.onPageEnd(ViewID);
		onPause();
	}

	static void onEventBegin(String eventId) {
		MobclickAgent.onEventBegin(_context, eventId);
	}

	static void onEventBeginValue(String eventId, String value) {
		MobclickAgent.onEventBegin(_context, eventId, value);
	}

	static void onEventEnd(String eventId) {
		MobclickAgent.onEventEnd(_context, eventId);
	}

	static void onEventEndValue(String eventId, String value) {
		MobclickAgent.onEventEnd(_context, eventId, value);
	}

	static void onEvent(String eventId) {
		MobclickAgent.onEvent(_context, eventId);
	}

	static void onEventValue(String eventId, String value) {
		MobclickAgent.onEventValue(_context, value, null, Integer.parseInt(value));
	}

	static void onEventMap(String eventId, String dicDesc) {
		try {
			JSONObject jObj = new JSONObject(dicDesc);
			HashMap<String, String> map = new HashMap<String, String>();

			Iterator it = jObj.keys();

			while (it.hasNext()) {
				String key = (String) it.next();
				String value = jObj.getString(key);
				map.put(key, value);
			}

			MobclickAgent.onEventValue(_context, eventId, map, 0);
		} catch (JSONException e) {
			e.printStackTrace();
			return;
		}
	}

	static void onEventMapValue(String eventId, String dicDesc, int v) {
		try {
			JSONObject jObj = new JSONObject(dicDesc);
			HashMap<String, String> map = new HashMap<String, String>();

			Iterator it = jObj.keys();

			while (it.hasNext()) {
				String key = (String) it.next();
				String value = jObj.getString(key);
				map.put(key, value);
			}

			MobclickAgent.onEventValue(_context, eventId, map, v);
		} catch (JSONException e) {
			e.printStackTrace();
			return;
		}
	}

	static String getOnlineParam(String key) {
		String param = MobclickAgent.getConfigParams(_context, key);
		return param;
	}

	static void showFeedback(final String uId) {
		// todo: need java 1.7
		 _context.runOnUiThread(new Runnable() {

		 @Override
		 public void run() {
		 FeedbackAgent agent = new FeedbackAgent(_context);
		 agent.startFeedbackActivity();

		 UserInfo info = agent.getUserInfo();
		 if (info == null)
		 info = new UserInfo();
		 Map<String, String> contact = info.getContact();
		 if (contact == null)
		 contact = new HashMap<String, String>();

		 contact.put("uId", uId);
		 info.setContact(contact);
		 agent.setUserInfo(info);
		 }
		 });
	}

	public static void onResume() {
		if (_init && _context != null) {
			MobclickAgent.onResume(_context);
			MobclickAgent.updateOnlineConfig(_context);
		}
	}

	public static void onPause() {
		if (_init && _context != null)
			MobclickAgent.onPause(_context);
	}
}
