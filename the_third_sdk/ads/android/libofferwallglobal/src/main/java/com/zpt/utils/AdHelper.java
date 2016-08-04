package com.zpt.utils;

import java.util.HashMap;
import java.util.Hashtable;
import com.nativex.monetization.MonetizationManager;
import com.nativex.monetization.communication.RedeemRewardData;
import com.nativex.monetization.enums.AdEvent;
import com.nativex.monetization.listeners.OnAdEventV2;
import com.nativex.monetization.listeners.RewardListener;
import com.nativex.monetization.listeners.SessionListener;
import com.nativex.monetization.mraid.AdInfo;
import com.supersonic.mediationsdk.logger.SupersonicError;
import com.supersonic.mediationsdk.sdk.OfferwallListener;
import com.supersonic.mediationsdk.sdk.Supersonic;
import com.supersonic.mediationsdk.sdk.SupersonicFactory;
import android.app.Activity;
import android.content.Context;
import net.adxmi.android.AdManager;
import net.adxmi.android.os.OffersManager;
import net.adxmi.android.video.VideoAdListener;
import net.adxmi.android.video.VideoAdManager;
import net.adxmi.android.video.VideoAdRequestListener;
import com.tapjoy.TJActionRequest;
import com.tapjoy.TJConnectListener;
import com.tapjoy.TJError;
import com.tapjoy.TJPlacement;
import com.tapjoy.TJPlacementListener;
import com.tapjoy.Tapjoy;
import com.tapjoy.TapjoyConnectFlag;
import com.zpt.libadscend.AdscendWrapper;
import com.zpt.libsuperrewards.SuperrewardsWrapper;

class MyNativeXListener implements OnAdEventV2, RewardListener, SessionListener {
	@Override
	public void createSessionCompleted(boolean success, boolean arg1, String arg2) {
		if (success) {
			// a session with our servers was established
			// successfully.
			// the app is now ready to show ads.
			System.out.println("Wahoo! Now I'm ready to show an ad.");

			MonetizationManager.fetchAd(AdHelper.getActivity(), AdHelper.K_NATIVEX_PLACEMENT_OFFER, this);
			MonetizationManager.fetchAd(AdHelper.getActivity(), AdHelper.K_NATIVEX_PLACEMENT_VIDEO, this);
		} else {
			// establishing a session with our servers failed;
			// the app will be unable to show ads until a session is
			// established
			System.out.println(
					"Oh no! Something isn't set up correctly - re-read the documentation or ask customer support for some help -"
							+ " https://selfservice.nativex.com/Help");
		}
	}

	@Override
	public void onRedeem(RedeemRewardData paramRedeemRewardData) {}

	@Override
	public void onEvent(AdEvent event, AdInfo adInfo, String message) {
		ZPTLog.v("Placement: " + adInfo.getPlacement());

		switch (event) {
		case ALREADY_SHOWN:
			// showAd() is called with an Ad Name and there
			// is an ad already being shown with the same
			// name at this moment.
			break;
		case BEFORE_DISPLAY:
			// Just before the Ad is displayed on the
			// screen.
			break;
		case DISMISSED:
			// The ad is dismissed by the user or by the
			// application.
			break;
		case DISPLAYED:
			// The ad is shown on the screen. For fetched
			// ads this event will fire when the showAd()
			// method is called.
			break;
		case DOWNLOADING:
			// fetchAd() is called with an Ad Name and there
			// is an ad already being fetched with the same
			// name at this moment.
			break;
		case ERROR:
			// An error has occurred and the ad is going to
			// be closed.
			// More information about the error is passed in
			// the "message" parameter.
			break;
		case EXPIRED:
			// A fetched ad expires. All fetched ads will
			// expire after a certain time period if not
			// shown.
			break;
		case ALREADY_FETCHED:
			// fetchAd() is called with an Ad Name and there
			// is already a fetched ad with the same name
			// ready to be shown.
		case FETCHED:
			// The ad is ready to be shown. For fetched ads
			// this method means that the ad is fetched
			// successfully.
			// You may want to initially put the
			// showReadyAd() call here when you're doing
			// your initial testing, but for production you
			// should move it to a more appropriate place,
			// as described in the Show an Ad section.
			String placement = adInfo.getPlacement();
			ZPTLog.v("Placement: " + placement);

			if (placement.equals(AdHelper.K_NATIVEX_PLACEMENT_OFFER)) {
				if (AdHelper._AdStep.get(AdHelper.K_NATIVEX_PLACEMENT_OFFER) == AdHelper.AD_READY) {
					AdHelper._AdStep.put(AdHelper.K_NATIVEX_PLACEMENT_OFFER, AdHelper.AD_FREE);
					MonetizationManager.showReadyAd(AdHelper.getActivity(), placement, this);

				} else {
					AdHelper._AdStep.put(AdHelper.K_NATIVEX_PLACEMENT_OFFER, AdHelper.AD_READY);
				}

			} else if (placement.equals(AdHelper.K_NATIVEX_PLACEMENT_VIDEO)) {
				if (AdHelper._AdStep.get(AdHelper.K_NATIVEX_PLACEMENT_VIDEO) == AdHelper.AD_READY) {
					AdHelper._AdStep.put(AdHelper.K_NATIVEX_PLACEMENT_VIDEO, AdHelper.AD_FREE);
					MonetizationManager.showReadyAd(AdHelper.getActivity(), placement, this);

				} else {
					AdHelper._AdStep.put(AdHelper.K_NATIVEX_PLACEMENT_VIDEO, AdHelper.AD_READY);
				}

			} else {
				ZPTLog.v("unknown placement");
				assert(false);
			}

			break;
		case NO_AD:
			// The device contacts the server, but there is
			// no ad ready to be shown at this time.
			break;
		case USER_NAVIGATES_OUT_OF_APP:
			// The user clicks on a link or a button in the
			// ad and is going to navigate out of the app
			// to the Google Play or a browser applications.
			break;
		case VIDEO_COMPLETED:
			// Video has completed playing; rewards will be
			// rewarded if applicable
			break;
		default:
			// Others do not apply to Interstitial ads.
			break;
		}
	}
}

class MySupersonicListener implements OfferwallListener {

	@Override
	public void onOfferwallShowFail(SupersonicError arg0) {
		ZPTLog.v("AdSupersonic:" + arg0.getErrorMessage());
	}

	@Override
	public void onOfferwallOpened() {
		ZPTLog.v("AdSupersonic:onOfferwallOpened");
	}

	@Override
	public void onOfferwallInitSuccess() {
		ZPTLog.v("AdSupersonic:onOfferwallInitSuccess");
	}

	@Override
	public void onOfferwallInitFail(SupersonicError arg0) {
		ZPTLog.v("AdSupersonic:" + arg0.getErrorMessage());
	}

	@Override
	public void onOfferwallClosed() {
		ZPTLog.v("AdSupersonic:onOfferwallClosed");
	}

	@Override
	public boolean onOfferwallAdCredited(int arg0, int arg1, boolean arg2) {
		ZPTLog.v("AdSupersonic:onOfferwallAdCredited");
		return true;
	}

	@Override
	public void onGetOfferwallCreditsFail(SupersonicError arg0) {
		ZPTLog.v("AdSupersonic:" + arg0.getErrorMessage());
	}
}

class MyTJPlacementListener implements TJPlacementListener {
	@Override
	public void onRequestSuccess(TJPlacement tjPlacement) {
		ZPTLog.v("onRequestSuccess:" + tjPlacement.getName());
	}

	@Override
	public void onRequestFailure(TJPlacement tjPlacement, TJError tjError) {

	}

	@Override
	public void onContentReady(TJPlacement tjPlacement) {
		ZPTLog.v("onContentReady:" + tjPlacement.getName());

		if (tjPlacement.getName().equals(AdHelper.K_TAPJOY_PLACEMENT_OFFER)) {
			if (AdHelper._AdStep.get(AdHelper.K_TAPJOY_PLACEMENT_OFFER).equals(AdHelper.AD_READY)) {
				AdHelper._AdStep.put(AdHelper.K_TAPJOY_PLACEMENT_OFFER, AdHelper.AD_FREE);
				tjPlacement.showContent();
			}
		}
	}

	@Override
	public void onContentShow(TJPlacement tjPlacement) {

	}

	@Override
	public void onContentDismiss(TJPlacement tjPlacement) {

	}

	@Override
	public void onPurchaseRequest(TJPlacement tjPlacement, TJActionRequest tjActionRequest, String s) {

	}

	@Override
	public void onRewardRequest(TJPlacement tjPlacement, TJActionRequest tjActionRequest, String s, int i) {

	}
}

public class AdHelper {
	public static final int AD_FREE = 0;
	public static final int AD_READY = 1;
	public static final String K_NATIVEX_PLACEMENT_OFFER = "Store Open Offerwall";
	public static final String K_NATIVEX_PLACEMENT_VIDEO = "Game Launch Video";
	public static final String K_TAPJOY_PLACEMENT_OFFER = "offerwall_unit";
	public static final String K_TAPJOY_PLACEMENT_VIDEO = "video_unit";

	public static HashMap<String, Integer> _AdStep = new HashMap<String, Integer>();

	private static final int OFFERWALL_REQUEST_CODE = 879510; // fyber

	private static Activity _activity = null;
	private static boolean _debug = false;
	private static TJPlacement _TJPlacementOfferwall;
	private static TJPlacement _TJPlacementVideo;

	private static Supersonic _mMediationAgent = null;
	private static MySupersonicListener _mySupersonicListener = new MySupersonicListener();
	private static MyNativeXListener _myNativeXListener = new MyNativeXListener();
	private static MyTJPlacementListener _myTJPlacementListener = new MyTJPlacementListener();

	class AdType {
		public static final int AdFyber = 0;
		public static final int AdSupersonic = 1;
		public static final int AdNativeX = 2;
		public static final int AdAdxmi = 3;
		public static final int AdTapjoy = 4;
		public static final int AdAdscend = 5;
		public static final int AdSuperrewards = 6;
	};

	public static void initContext(Activity activity, boolean debug) {
		_activity = activity;
		_debug = debug;
	}

	public static Activity getActivity() {
		return _activity;
	}

	public static void initAd(final int type, final String uId, final String appkey, final String token) {
		ZPTLog.d("init ad:" + type);

		if (_activity == null) {
			assert(false);
			return;
		}

		_activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
					case AdType.AdFyber:
//						SponsorPay.start(appkey, uId, token, _activity);
						break;
					case AdType.AdSupersonic:
						_mMediationAgent = SupersonicFactory.getInstance();
						_mMediationAgent.setOfferwallListener(_mySupersonicListener);
						_mMediationAgent.initOfferwall(_activity, appkey, uId);
						_mMediationAgent.initRewardedVideo(_activity, appkey, uId);
						break;
					case AdType.AdNativeX:
						_AdStep.put(K_NATIVEX_PLACEMENT_OFFER, AD_FREE);
						_AdStep.put(K_NATIVEX_PLACEMENT_VIDEO, AD_FREE);

						MonetizationManager.enableLogging(_debug);
						MonetizationManager.createSession(_activity, appkey, uId, _myNativeXListener);
						break;
					case AdType.AdAdxmi:
						AdManager.getInstance(_activity).setEnableDebugLog(_debug);
						AdManager.getInstance(_activity).init(appkey, token);

						OffersManager.getInstance(_activity).setCustomUserId(uId);
						VideoAdManager.getInstance(_activity).setCustomUserId(uId);
						OffersManager.getInstance(_activity).onAppLaunch();
						// init video
						VideoAdManager.getInstance(_activity).requestVideoAd(new VideoAdRequestListener() {

							@Override
							public void onRequestSucceed() {
								ZPTLog.v("videoPlay 请求成功");
							}

							@Override
							public void onRequestFail(int errorCode) {
								// 关于错误码的解读：-1 为网络连接失败，请检查网络。-2007 为无广告，-3312
								// 为该设备一天的播放次数已完，其他错误码一般为设备问题。
								ZPTLog.v("videoPlay", "请求失败，错误码为:" + errorCode);
							}
						});
						break;
					case AdType.AdTapjoy:
						Hashtable<String, Object> connectFlags = new Hashtable<String, Object>();
						connectFlags.put(TapjoyConnectFlag.ENABLE_LOGGING, _debug ? "true" : "false");
						connectFlags.put(TapjoyConnectFlag.STORE_NAME, "Google Play");
						connectFlags.put(TapjoyConnectFlag.USER_ID, uId);

						AdHelper._AdStep.put(AdHelper.K_TAPJOY_PLACEMENT_OFFER, AdHelper.AD_FREE);
						AdHelper._AdStep.put(AdHelper.K_TAPJOY_PLACEMENT_VIDEO, AdHelper.AD_FREE);

						Tapjoy.setDebugEnabled(_debug);

						Tapjoy.connect(_activity, appkey, connectFlags, new TJConnectListener() {

							@Override
							public void onConnectSuccess() {
								ZPTLog.d("tapjoy onConnectSuccess");

								_TJPlacementOfferwall = new TJPlacement(_activity, K_TAPJOY_PLACEMENT_OFFER, _myTJPlacementListener);
								_TJPlacementOfferwall.requestContent();

								_TJPlacementVideo = new TJPlacement(_activity, "video_unit", _myTJPlacementListener);
								_TJPlacementVideo.requestContent();
							}

							@Override
							public void onConnectFailure() {
								ZPTLog.d("tapjoy onConnectFailure");
							}
						});
						break;
					case AdType.AdAdscend:
						AdscendWrapper.init(_activity, uId, appkey, token);
						break;
					case AdType.AdSuperrewards:
						SuperrewardsWrapper.init(_activity, uId, appkey);
						break;
					default:
						ZPTAssert.check(false, "no offerwall of this key");
						break;
				}
			}
		});
	}

	public static void callOfferwall(final int type) {
		final Context context = _activity;

		_activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
					case AdType.AdFyber:
//						_activity.startActivityForResult(
//								SponsorPayPublisher.getIntentForOfferWallActivity(_activity, true),
//								OFFERWALL_REQUEST_CODE);
						break;
					case AdType.AdSupersonic:
						if (_mMediationAgent != null && _mMediationAgent.isOfferwallAvailable()) {
							_mMediationAgent.showOfferwall();
						}
						break;
					case AdType.AdNativeX:
						_AdStep.put(K_NATIVEX_PLACEMENT_OFFER, AD_READY);

						if (MonetizationManager.isAdReady(K_NATIVEX_PLACEMENT_OFFER)) {
							_AdStep.put(K_NATIVEX_PLACEMENT_OFFER, AD_FREE);
							MonetizationManager.showAd(_activity, K_NATIVEX_PLACEMENT_OFFER,
									_myNativeXListener);

						} else {
							MonetizationManager.fetchAd(_activity, K_NATIVEX_PLACEMENT_OFFER,
									_myNativeXListener);
						}
						break;
					case AdType.AdAdxmi:
						OffersManager.getInstance(context).showOffersWall();
						break;
					case AdType.AdTapjoy:
						if (_TJPlacementOfferwall != null) {
							if (_TJPlacementOfferwall.isContentReady()) {
								_TJPlacementOfferwall.showContent();
							} else {
								ZPTLog.v("_TJPlacementOfferwall show: " + _TJPlacementOfferwall.isContentAvailable());
								AdHelper._AdStep.put(AdHelper.K_TAPJOY_PLACEMENT_OFFER, AdHelper.AD_READY);
								_TJPlacementOfferwall.requestContent();
							}
						} else {
							ZPTLog.v("_TJPlacementOfferwall is null");
						}
						break;
					case AdType.AdAdscend:
						AdscendWrapper.showOfferwall();
						break;
					case AdType.AdSuperrewards:
						SuperrewardsWrapper.showOfferwall();
						break;
					default:
						assert (false);
						break;
				}
			}
		});

	}

	public static void playVideo(final int type) {
		_activity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
					case AdType.AdFyber:
						break;
					case AdType.AdSupersonic:
						if (_mMediationAgent != null && _mMediationAgent.isRewardedVideoAvailable()) {
							_mMediationAgent.showRewardedVideo();
						}
						break;
					case AdType.AdNativeX:
						_AdStep.put(K_NATIVEX_PLACEMENT_VIDEO, AD_READY);

						if (MonetizationManager.isAdReady(K_NATIVEX_PLACEMENT_VIDEO)) {
							_AdStep.put(K_NATIVEX_PLACEMENT_VIDEO, AD_FREE);
							MonetizationManager.showAd(_activity, K_NATIVEX_PLACEMENT_VIDEO,
									_myNativeXListener);

						} else {
							MonetizationManager.fetchAd(_activity, K_NATIVEX_PLACEMENT_VIDEO,
									_myNativeXListener);
						}
						break;
					case AdType.AdAdxmi:
						VideoAdManager.getInstance(_activity).showVideo(_activity, new VideoAdListener() {

							@Override
							public void onVideoPlayFail() {
								ZPTLog.v("videoPlay", "failed");
							}

							@Override
							public void onVideoPlayComplete() {
								ZPTLog.v("videoPlay", "complete");
							}

							@Override
							public void onVideoCallback(boolean callback) {
								// 视频播放记录发送是否回调成功
								ZPTLog.v("videoPlay", "completeEffect:" + callback);
							}

							@Override
							public void onVideoPlayInterrupt() {
								ZPTLog.v("videoPlay", "interrupt");
							}
						});
						break;
					case AdType.AdTapjoy:
						break;
					default:
						assert (false);
						break;
				}
			}
		});
	}

	public static void onStart(Activity ctx) {
		Tapjoy.onActivityStart(ctx);
	}

	public static void onStop(Activity ctx) {
		Tapjoy.onActivityStop(ctx);
	}

	public static void onResume(Activity ctx) {
		if (_mMediationAgent != null) {
			_mMediationAgent.onResume(ctx);
		}
	}

	public static void onPause(Activity ctx) {
		if (_mMediationAgent != null) {
			_mMediationAgent.onPause(ctx);
		}
	}
}
