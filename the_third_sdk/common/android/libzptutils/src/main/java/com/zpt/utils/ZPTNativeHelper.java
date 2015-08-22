package com.zpt.utils;

import java.util.HashMap;
import java.util.Locale;

import org.CrossApp.lib.Cocos2dxActivity;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.SystemClock;
import android.provider.Settings;
import android.telephony.TelephonyManager;

public class ZPTNativeHelper {

	public static String getDeviceInfo() {
		Context ctx = Cocos2dxActivity.getContext();
		PackageManager packageManager = ctx.getPackageManager();
		PackageInfo packageInfo;

		HashMap<String, String> data = new HashMap<String, String>();
		data.put("name", android.os.Build.MODEL);
		data.put("os", android.os.Build.VERSION.RELEASE);
		data.put("sdk_int", android.os.Build.VERSION.SDK_INT + "");

		try {
			packageInfo = packageManager.getPackageInfo(ctx.getPackageName(), 0);
			data.put("app_version", packageInfo.versionName);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			data.put("app_version", "");
		}

		return new JSONObject(data).toString();
	}

	public static void openUrl(String url) {
		final Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));

		Cocos2dxActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Cocos2dxActivity.getContext().startActivity(Intent.createChooser(intent, "Open With"));

			}
		});
	}

	public static void sendMail(String target, String title, String extraDesc) {
		final Intent data = new Intent(Intent.ACTION_SENDTO);
		data.setData(Uri.parse("mailto:" + target));
		data.putExtra(Intent.EXTRA_SUBJECT, title);
		data.putExtra(Intent.EXTRA_TEXT, extraDesc);

		Cocos2dxActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Cocos2dxActivity.getContext().startActivity(data);
			}
		});
	}

	public static String getDeviceId() {
		final TelephonyManager tm = (TelephonyManager) Cocos2dxActivity.getContext().getBaseContext()
				.getSystemService(Context.TELEPHONY_SERVICE);

		String uId = tm.getDeviceId();

		if (uId == null || uId.equals("")) {
			uId = Settings.Secure.getString(Cocos2dxActivity.getContext().getContentResolver(), Settings.Secure.ANDROID_ID);
		}

		if (uId == null || uId.equals("")) {
			WifiManager wifiManager = (WifiManager) Cocos2dxActivity.getContext().getSystemService(Context.WIFI_SERVICE);
			WifiInfo wifiInfo = wifiManager.getConnectionInfo();

			if (wifiInfo.getMacAddress() != null) {
				uId = wifiInfo.getMacAddress();
			}
		}

		if (uId == null || uId.equals("")) {
			assert (false);
			uId = "";
		}

		ZPTLog.v("DeviceId:" + uId);
		return uId;
	}

	public static String getAppVersion() {
		Context ctx = Cocos2dxActivity.getContext();
		PackageManager packageManager = ctx.getPackageManager();
		PackageInfo packageInfo;

		String appversion = "";

		try {
			packageInfo = packageManager.getPackageInfo(ctx.getPackageName(), 0);
			appversion = packageInfo.versionName;
		} catch (NameNotFoundException e) {
			assert(false);
			e.printStackTrace();
		}

		return appversion;
	}

	public static int getAppBuild() {
		Context ctx = Cocos2dxActivity.getContext();
		PackageManager packageManager = ctx.getPackageManager();
		PackageInfo packageInfo;
		int appBuild = 0;

		try {
			packageInfo = packageManager.getPackageInfo(ctx.getPackageName(), 0);
			appBuild = packageInfo.versionCode;
		} catch (NameNotFoundException e) {
			assert(false);
			e.printStackTrace();
		}

		return appBuild;
	}

	public static String getLanguage() {
		String language = Locale.getDefault().getLanguage();

		if (language.equals("zh")) {
			language = Locale.getDefault().toString();

			if (language.equals("zh_TW")) {
				language = "zh-Hant";
			} else {
				language = "zh-Hans";
			}
		}

		return language;
	}

	public static String getCountyCode() {
		return Locale.getDefault().toString();
	}

	/**
	 * 获取手机信息
	 */
	public static String getPhoneInfo() {
		Context ctx = Cocos2dxActivity.getContext();
		TelephonyManager tm = (TelephonyManager) ctx.getSystemService(Context.TELEPHONY_SERVICE);
		String deviceBrand = android.os.Build.BRAND; // 手机品牌
		String deviceModel = android.os.Build.MODEL; // 手机型号
		String imei = tm.getDeviceId();
		String imsi = tm.getSubscriberId();
		String numer = tm.getLine1Number(); // 手机号码
		String serviceName = tm.getSimOperatorName(); // 运营商

		String macStr, time;
		WifiManager wifiManager = (WifiManager) ctx.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();

		if (wifiInfo.getMacAddress() != null) {

			macStr = wifiInfo.getMacAddress();
		} else {
			macStr = "Fail";
		}

		time = SystemClock.elapsedRealtime() / 1000 + "";

		String phoneInfo = "品牌: " + deviceBrand
				+ "\n" + "型号: " + deviceModel
				+ "\n" + "版本: Android " + android.os.Build.VERSION.RELEASE
				+ "\n" + "IMEI: " + imei
				+ "\n" + "IMSI: " + imsi
				+ "\n" + "手机号码: " + numer
				+ "\n" + "运营商: " + serviceName
				+ "\n" + "mac: " + macStr
				+ "\n" + "time: " + time;
		
		ZPTLog.v("phoneInfo: " + phoneInfo);
		
		HashMap<String, String> infoMap = new HashMap<String, String>();
		infoMap.put("brand", deviceBrand);
		infoMap.put("model", deviceModel);
		infoMap.put("system", "Android " + android.os.Build.VERSION.RELEASE);
		infoMap.put("imei", imei);
		infoMap.put("imsi", imsi);
		infoMap.put("numer", numer);
		infoMap.put("service", serviceName);
		infoMap.put("mac", macStr);
		infoMap.put("time", time);
		
		JSONObject jsonObj = new JSONObject(infoMap);

		return jsonObj.toString();
	}

}
