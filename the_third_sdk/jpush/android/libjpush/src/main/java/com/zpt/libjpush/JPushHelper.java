package com.zpt.libjpush;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.HashSet;
import com.zpt.utils.ZPTLog;
import android.app.Activity;
import android.content.Context;
import cn.jpush.android.api.BasicPushNotificationBuilder;
import cn.jpush.android.api.JPushInterface;
import com.google.gson.Gson;

import org.CrossApp.lib.CrossAppHelper;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class JPushHelper {

	private static Activity _ctx = null;
	private static int _icon;
	private final static String JPUSH_NOTI_PATH = "jpush_notis.json";
	private final static String KEY_JPUSH_NOTIS = "notis";
	
	public static void initWithContext(Activity ctx, boolean debug, int icon) {


		JPushInterface.setDebugMode(debug);
		_ctx = ctx;
		_icon = icon;
	}
	
	public static void initUser(String uId, HashSet<String> tags) {
		if (_ctx == null) {
			assert(false);
			return;
		}

		ZPTLog.v("tags:" + tags.toString());

		JPushInterface.init(_ctx);
		ZPTJPushReceiver.init(_ctx.getClass());
		JPushInterface.setAliasAndTags(_ctx, uId, tags);

		BasicPushNotificationBuilder basicPushNotificationBuilder = new BasicPushNotificationBuilder(_ctx);
		basicPushNotificationBuilder.statusBarDrawable = _icon;
		JPushInterface.setDefaultPushNotificationBuilder(basicPushNotificationBuilder);
	}

	public static void onResume(Activity ctx) {
		JPushInterface.onResume(ctx);
	}

	public static void onPause(Activity ctx) {
		JPushInterface.onPause(ctx);
	}

	// used in remote process
	/**
	 *  {
	 *      notis: [{
	 *
	 *      },{}..]
	 *  }
	 *
	 *
	 */

	public static void saveNoti(Context ctx, String data) {
		//队列,max 5
		String fullPath = JPUSH_NOTI_PATH;
		final int NOTI_MAX = 5;

		String fileData = "";
		try{
			FileInputStream fin = ctx.openFileInput(fullPath);
			int length = fin.available();
			byte[] buffer = new byte[length];
			fin.read(buffer);
			fileData = new String(buffer, "UTF-8");
			fin.close();
		} catch(Exception e){
			e.printStackTrace();
			fileData = "";
		}

		ZPTLog.d("[jpush_helper] read:" + fileData);

		JSONObject json;
		try {
			json = new JSONObject(fileData);
		} catch (Exception e) {
			e.printStackTrace();
			json = new JSONObject();
		}

		try {
			if (json.isNull(KEY_JPUSH_NOTIS) || json.getJSONArray(KEY_JPUSH_NOTIS).length() == 0) {
				json.put(KEY_JPUSH_NOTIS, new JSONArray());
			}

			//队列max
			JSONArray notis = json.getJSONArray(KEY_JPUSH_NOTIS);
			JSONObject dataInJson = new JSONObject(data);
			notis.put(dataInJson);

			ZPTLog.d("[jpush_helper] notis length: " + notis.length());

			if (notis.length() > NOTI_MAX) {
				JSONArray temArry = new JSONArray();
				for (int i = 0; i < NOTI_MAX; i++) {
					temArry.put(notis.getJSONObject(i+1));

					ZPTLog.d("[jpush_helper] temp:" + temArry.toString());
				}

				notis = temArry;
				json.put(KEY_JPUSH_NOTIS, notis);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		String fileStr = json.toString();
		try {
			ZPTLog.d("[jpush_helper] write:" + fileStr);

			FileOutputStream fout = ctx.openFileOutput(fullPath, Context.MODE_PRIVATE);
			fout.write(fileStr.getBytes("UTF-8"));
			fout.close();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

	// used in main process
	public static void readNoti(Context ctx) throws JSONException {
		//队列,max 5
		String fullPath = JPUSH_NOTI_PATH;
		final int NOTI_MAX = 5;

		String fileData = "";
		try{
			FileInputStream fin = ctx.openFileInput(fullPath);
			int length = fin.available();
			byte[] buffer = new byte[length];
			fin.read(buffer);
			fileData = new String(buffer, "UTF-8");
			fin.close();
		} catch(Exception e){
			e.printStackTrace();
			fileData = "";
		}

		ZPTLog.d("[jpush_helper] read:" + fileData);

		JSONObject json;
		try {
			json = new JSONObject(fileData);
		} catch (Exception e) {
			e.printStackTrace();
			ZPTLog.d("[jpush_helper] read empty file.");
			return;
		}

		try {
			if (json.isNull(KEY_JPUSH_NOTIS) || json.getJSONArray(KEY_JPUSH_NOTIS).length() == 0) {
				return;
			}

			JSONArray notis = json.getJSONArray(KEY_JPUSH_NOTIS);

			for (int i = 0; i < json.length(); i++) {
				JSONObject noti = notis.getJSONObject(i);
				//broadcast
				CrossAppHelper.postNotification(CrossAppHelper.kNotiRemoteNotiRecived, noti.toString());
			}

			json.remove(KEY_JPUSH_NOTIS);

		} catch (Exception e) {
			e.printStackTrace();
			json = new JSONObject();
		}

		String fileStr = json.toString();
		try {
			ZPTLog.d("[jpush_helper] write:" + fileStr);

			FileOutputStream fout = ctx.openFileOutput(fullPath, Context.MODE_PRIVATE);
			fout.write(fileStr.getBytes("UTF-8"));
			fout.close();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
