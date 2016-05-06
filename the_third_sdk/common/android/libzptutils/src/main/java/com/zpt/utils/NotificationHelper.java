package com.zpt.utils;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;

public class NotificationHelper extends BroadcastReceiver {
	static Activity mContext = null;
	static int mIconID = 0;
	static NotificationManager mNotificationManager = null;
	static String mAlarmAction = null;
	static boolean mInit = false;

	public static void init(Activity context, String action, int icon) {
		mContext = context;
		setIconID(icon);
		setAlarmAction(action);

		mNotificationManager = (NotificationManager) mContext.getSystemService(Context.NOTIFICATION_SERVICE);
		mInit = true;
	}

	public static void setIconID(int iconID) {
		mIconID = iconID;
	}

	public static void setAlarmAction(String action) {
		mAlarmAction = action;
	}

	public static void cancelAllLocalNotification() {
		ZPTLog.e("Android not support cancel all notification in once.");
	}

	public static void cancelLocalNotification(int requestCode) {
		if (!mInit) {
			ZPTLog.e("should init first.");
			assert(false);
			return;
		}

		Intent alarmIntent = new Intent();
		alarmIntent.setAction(mAlarmAction);

		PendingIntent alarmPending = PendingIntent.getBroadcast(mContext, requestCode, alarmIntent,
				PendingIntent.FLAG_NO_CREATE);
		AlarmManager alarmManager = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);
		alarmManager.cancel(alarmPending);

		NotificationManager notificationManager = (NotificationManager) mContext
				.getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.cancel(requestCode);
	}

	/**
	 * dict 为notification的一些数据 alertTitle alertBody requestCode timestamp long
	 * alertTime: second
	 * interval: second
	 */
	public static void createLocalNotification(String alertTitle, String alertBody, long alertTime, int requestCode, long repeatInterval) {
		if (!mInit) {
			ZPTLog.e("should init first.");
			return;
		}

		ZPTLog.d("---", alertTitle + "," + alertBody + "," + alertTime + "," + requestCode + "," + repeatInterval);

		alertTime = alertTime * 1000;
		repeatInterval = repeatInterval * 1000;

		Intent alarmIntent = new Intent();
		alarmIntent.setAction(mAlarmAction);
		alarmIntent.putExtra("alertTitle", alertTitle);
		alarmIntent.putExtra("alertBody", alertBody);
		alarmIntent.putExtra("alertTime", alertTime);
		alarmIntent.putExtra("requestCode", requestCode);
		alarmIntent.putExtra("NOTIFICATION_ICON", mIconID);

		PendingIntent alarmPending = PendingIntent.getBroadcast(mContext, requestCode, alarmIntent,
				PendingIntent.FLAG_UPDATE_CURRENT);
		AlarmManager alarmManager = (AlarmManager) mContext.getSystemService(Context.ALARM_SERVICE);

		if (repeatInterval > 0) {
			alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, alertTime, repeatInterval, alarmPending);
		} else {
			alarmManager.set(AlarmManager.RTC_WAKEUP, alertTime, alarmPending);
		}
	}

	// ///////////// BroadcastReceiver functions

	// class

	// 收到闹钟消息后发送notification
	@SuppressWarnings("deprecation")
	@SuppressLint("NewApi")
	@Override
	public void onReceive(Context context, Intent intent) {
		Bundle bundle = intent.getExtras();

		if (bundle == null)
			return;

		int icon = bundle.getInt("NOTIFICATION_ICON");
		String alertTitle = bundle.getString("alertTitle");
		String alertBody = bundle.getString("alertBody");
		int requestCode = bundle.getInt("requestCode");

		PackageManager pm = context.getPackageManager();
		String pkgName = context.getPackageName();
		Intent notifyIntent= pm.getLaunchIntentForPackage(pkgName);
		notifyIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);

		// 创建notification
		// 点击通知后的动作，启动游戏
		PendingIntent pt = PendingIntent.getActivity(context, requestCode, notifyIntent,
				PendingIntent.FLAG_UPDATE_CURRENT);

		Notification notification = null;

		try {
			notification = new Notification.Builder(context).setSmallIcon(icon).setContentTitle(alertTitle)
					.setContentText(alertBody).setContentIntent(pt).setTicker(alertBody).build();
		} catch (NoClassDefFoundError e) {
			e.printStackTrace();
			// 老api
//			notification = new Notification(icon, alertBody, System.currentTimeMillis());
//			notification.setLatestEventInfo(context, alertTitle, alertBody, pt);
		} catch (NoSuchMethodError e) {
			e.printStackTrace();
			// 老api
//			notification = new Notification(icon, alertBody, System.currentTimeMillis());
//			notification.setLatestEventInfo(context, alertTitle, alertBody, pt);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}

		notification.flags = Notification.FLAG_AUTO_CANCEL;
		// 这是设置通知是否同时播放声音或振动，声音为Notification.DEFAULT_SOUND
		// 振动为Notification.DEFAULT_VIBRATE
		// Light为Notification.DEFAULT_LIGHTS
		// 全部为Notification.DEFAULT_ALL
		// 如果是振动或者全部，必须在AndroidManifest.xml加入振动权限
		notification.defaults = Notification.DEFAULT_ALL;
		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.notify(requestCode, notification);
	}
}
