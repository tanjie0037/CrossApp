package com.zpt.libjpush;

import java.util.HashSet;
import com.zpt.utils.ZPTLog;
import android.app.Activity;

import cn.jpush.android.api.BasicPushNotificationBuilder;
import cn.jpush.android.api.JPushInterface;

public class JPushHelper {

	private static Activity _ctx = null;
	private static int _icon;
	
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
}
