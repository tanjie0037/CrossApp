package com.zpt.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Locale;
import org.CrossApp.lib.CrossAppActivity;
import org.json.JSONException;
import org.json.JSONObject;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.*;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.widget.Toast;

import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.auth.GooglePlayServicesAvailabilityException;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;

public class ZPTNativeHelper {
	public static String _adid = null;

	public static String getDeviceInfo() {
		Context ctx = CrossAppActivity.getContext();
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

		CrossAppActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				CrossAppActivity.getContext().startActivity(Intent.createChooser(intent, "Open With"));

			}
		});
	}

	public static void sendMail(String target, String title, String extraDesc) {
		final Intent data = new Intent(Intent.ACTION_SENDTO);
		data.setData(Uri.parse("mailto:" + target));
		data.putExtra(Intent.EXTRA_SUBJECT, title);
		data.putExtra(Intent.EXTRA_TEXT, extraDesc);

		CrossAppActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				CrossAppActivity.getContext().startActivity(data);
			}
		});
	}

	public static String getDeviceId() {
		final TelephonyManager tm = (TelephonyManager) CrossAppActivity.getContext().getBaseContext()
				.getSystemService(Context.TELEPHONY_SERVICE);

		String uId = tm.getDeviceId();

		if (uId == null || uId.equals("")) {
			uId = Settings.Secure.getString(CrossAppActivity.getContext().getContentResolver(), Settings.Secure.ANDROID_ID);
		}

		if (uId == null || uId.equals("")) {
			WifiManager wifiManager = (WifiManager) CrossAppActivity.getContext().getSystemService(Context.WIFI_SERVICE);
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
		Context ctx = CrossAppActivity.getContext();
		PackageManager packageManager = ctx.getPackageManager();
		PackageInfo packageInfo;

		String appversion = "";

		try {
			packageInfo = packageManager.getPackageInfo(ctx.getPackageName(), 0);
			appversion = packageInfo.versionName;
		} catch (NameNotFoundException e) {
			assert (false);
			e.printStackTrace();
		}

		return appversion;
	}

	public static int getAppBuild() {
		Context ctx = CrossAppActivity.getContext();
		PackageManager packageManager = ctx.getPackageManager();
		PackageInfo packageInfo;
		int appBuild = 0;

		try {
			packageInfo = packageManager.getPackageInfo(ctx.getPackageName(), 0);
			appBuild = packageInfo.versionCode;
		} catch (NameNotFoundException e) {
			assert (false);
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

	public static String getCountryCode() {
		return Locale.getDefault().toString();
	}

	/**
	 * 获取手机信息
	 */
	public static String getPhoneInfo() {
		Context ctx = CrossAppActivity.getContext();
		TelephonyManager tm = (TelephonyManager) ctx.getSystemService(Context.TELEPHONY_SERVICE);
		String deviceBrand = android.os.Build.BRAND; // 手机品牌
		String deviceModel = android.os.Build.MODEL; // 手机型号
		String imei = tm.getDeviceId();
		String imsi = tm.getSubscriberId();
		String nubmer = tm.getLine1Number(); // 手机号码
		String serviceName = tm.getSimOperatorName(); // 运营商
		String androidId = Settings.Secure.getString(CrossAppActivity.getContext().getContentResolver(), Settings.Secure.ANDROID_ID);

		String macStr, time;
		WifiManager wifiManager = (WifiManager) ctx.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();

		if (wifiInfo.getMacAddress() != null) {

			macStr = wifiInfo.getMacAddress();
		} else {
			macStr = "Fail";
		}

		time = SystemClock.elapsedRealtime() / 1000 + "";

		HashMap<String, String> infoMap = new HashMap<String, String>();
		infoMap.put("brand", deviceBrand);
		infoMap.put("model", deviceModel);
		infoMap.put("system", "Android " + android.os.Build.VERSION.RELEASE);
		infoMap.put("imei", imei);
		infoMap.put("imsi", imsi);
		infoMap.put("number", nubmer);
		infoMap.put("service", serviceName);
		infoMap.put("mac", macStr);
		infoMap.put("time", time);
		infoMap.put("adid", _adid);
		infoMap.put("android_id", androidId);

		JSONObject jsonObj = new JSONObject(infoMap);

		ZPTLog.v("privateInfo:\n" + jsonObj.toString());

		return jsonObj.toString();
	}

	public static void getAdid() {

		new AsyncTask() {
			@Override
			protected Object doInBackground(Object[] params) {
				AdvertisingIdClient.Info adInfo = null;
				Context ctx = CrossAppActivity.getContext();

				try {

					adInfo = AdvertisingIdClient.getAdvertisingIdInfo(ctx);
					_adid = adInfo.getId();
				} catch (IOException e) {
					// Unrecoverable error connecting to Google Play services (e.g.,
					// the old version of the service doesn't support getting AdvertisingId).
				} catch (GooglePlayServicesNotAvailableException e) {
					// Google Play services is not available entirely.
					e.printStackTrace();
				} catch (GooglePlayServicesRepairableException e) {
					e.printStackTrace();
				}

				return null;
			}
		}.execute();

	}

	public static void callGoldMine(final String packageName, final String activityName, final String intentName, final String params) {
		final CrossAppActivity ctx = CrossAppActivity.getContext();

		ctx.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					ComponentName componetName = new ComponentName(
							//这个是另外一个应用程序的包名
							packageName,
							//这个参数是要启动的Activity
							activityName);
					Intent intent = new Intent(intentName);
					Bundle bundle = new Bundle();
					bundle.putString("data", params);
					intent.putExtras(bundle);
					intent.setComponent(componetName);
					ctx.startActivity(intent);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public static void closeApp() {
//		//// TODO: need to rewrite
//		String goldminePkg = "com.zero.diaobaole";
//		String activityName = "com.zero.diaobaole.MainActivity";
//		String intentName = "com.diao.diaobaole.OFFERWALL";
//
//		JSONObject data = new JSONObject();
//		try {
//			data.put("function", "exit");
//			callGoldMine(goldminePkg, activityName, intentName, data.toString());
//
//		} catch (JSONException e) {
//			e.printStackTrace();
//		}

		new AsyncTask() {
			@Override
			protected Object doInBackground(Object[] params) {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

				android.os.Process.killProcess(android.os.Process.myPid());
				return null;
			}
		}.execute();
	}

	public static boolean isAppInstalled(String packageName, int requiredBuild) {
		if (packageName == null || "".equals(packageName))
			return false;

		PackageManager packageManager = CrossAppActivity.getContext().getPackageManager();

		try {
			PackageInfo packageInfo = packageManager.getPackageInfo(packageName, 0);
			if (packageInfo != null && packageInfo.versionCode >= requiredBuild) {
				return true;
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return false;
	}

	public static void installApk(String apkName) {
		Context ctx = CrossAppActivity.getContext();
		String apkPath = getCacheDir(ctx) + "/" + apkName;

		if (copyApkFromAssets(ctx, apkName, apkPath)) {
			Uri uri = Uri.fromFile(new File(apkPath));

			Intent intent = new Intent(Intent.ACTION_INSTALL_PACKAGE);
			intent.setData(uri);
			intent.putExtra(Intent.EXTRA_NOT_UNKNOWN_SOURCE, true);
			intent.putExtra(Intent.EXTRA_RETURN_RESULT, true);
			intent.putExtra(Intent.EXTRA_INSTALLER_PACKAGE_NAME, ctx.getApplicationInfo().packageName);

			ctx.startActivity(intent);
		}
	}

	public static boolean fileIsExists(String strFile) {
		try {
			File f = new File(strFile);
			if(!f.exists()) {
				return false;
			}

		} catch (Exception e) {
			return false;
		}

		return true;
	}

	public static boolean copyApkFromAssets(Context context, String fileName, String path) {
		boolean copyIsFinish = false;

		try {
			InputStream is = context.getAssets().open(fileName);
			File file = new File(path);
			file.createNewFile();
			FileOutputStream fos = new FileOutputStream(file);
			byte[] temp = new byte[1024];
			int i = 0;
			while ((i = is.read(temp)) > 0) {
				fos.write(temp, 0, i);
			}
			fos.close();
			is.close();

			copyIsFinish = true;

		} catch (IOException e) {
			e.printStackTrace();
			Toast.makeText(context, "storage is in used, please close other apps and try again.", Toast.LENGTH_SHORT).show();
		}

		return copyIsFinish;
	}

	public static String getCacheDir(Context context) {
		String cachePath;

		if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
				|| !Environment.isExternalStorageRemovable()) {
			cachePath = context.getExternalCacheDir().getPath();
		} else {
			cachePath = context.getCacheDir().getPath();
		}

		return cachePath;
	}
}
