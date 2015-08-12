package com.zpt.utils;

import java.util.HashMap;
import org.CrossApp.lib.Cocos2dxActivity;
import com.nativex.monetization.MonetizationManager;
import com.nativex.monetization.communication.RedeemRewardData;
import com.nativex.monetization.enums.AdEvent;
import com.nativex.monetization.listeners.OnAdEventV2;
import com.nativex.monetization.listeners.RewardListener;
import com.nativex.monetization.listeners.SessionListener;
import com.nativex.monetization.mraid.AdInfo;
import com.sponsorpay.SponsorPay;
import com.sponsorpay.publisher.SponsorPayPublisher;
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
import com.zpt.utils.RR;
import com.zpt.utils.ZPTLog;

//import com.tapjoy.TJActionRequest;
//import com.tapjoy.TJConnectListener;
//import com.tapjoy.TJError;
//import com.tapjoy.TJPlacement;
//import com.tapjoy.TJPlacementListener;
//import com.tapjoy.Tapjoy;
//import com.tapjoy.TapjoyConnectFlag;

class MyNativeXListener implements OnAdEventV2, RewardListener, SessionListener {
	@Override
	public void createSessionCompleted(boolean success, boolean arg1, String arg2) {
		if (success) {
			// a session with our servers was established
			// successfully.
			// the app is now ready to show ads.
			System.out.println("Wahoo! Now I'm ready to show an ad.");

			MonetizationManager.fetchAd(Cocos2dxActivity.getContext(), AdHelper.K_NATIVEX_PLACEMENT_OFFER, this);
			MonetizationManager.fetchAd(Cocos2dxActivity.getContext(), AdHelper.K_NATIVEX_PLACEMENT_VIDEO, this);
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
	public void onRedeem(RedeemRewardData paramRedeemRewardData) {
		// TODO Auto-generated method stub
	}

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
				if (AdHelper._nativeXStep.get(AdHelper.K_NATIVEX_PLACEMENT_OFFER) == AdHelper.NX_READY) {
					AdHelper._nativeXStep.put(AdHelper.K_NATIVEX_PLACEMENT_OFFER, AdHelper.NX_FREE);
					MonetizationManager.showReadyAd(Cocos2dxActivity.getContext(), placement, this);

				} else {
					AdHelper._nativeXStep.put(AdHelper.K_NATIVEX_PLACEMENT_OFFER, AdHelper.NX_READY);
				}

			} else if (placement.equals(AdHelper.K_NATIVEX_PLACEMENT_VIDEO)) {
				if (AdHelper._nativeXStep.get(AdHelper.K_NATIVEX_PLACEMENT_VIDEO) == AdHelper.NX_READY) {
					AdHelper._nativeXStep.put(AdHelper.K_NATIVEX_PLACEMENT_VIDEO, AdHelper.NX_FREE);
					MonetizationManager.showReadyAd(Cocos2dxActivity.getContext(), placement, this);

				} else {
					AdHelper._nativeXStep.put(AdHelper.K_NATIVEX_PLACEMENT_VIDEO, AdHelper.NX_READY);
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

public class AdHelper {
	public static final int NX_FREE = 0;
	public static final int NX_READY = 1;
	public static final String K_NATIVEX_PLACEMENT_OFFER = "Store Open Offerwall";
	public static final String K_NATIVEX_PLACEMENT_VIDEO = "Game Launch Video";

	public static HashMap<String, Integer> _nativeXStep = new HashMap<String, Integer>();

	private static final int OFFERWALL_REQUEST_CODE = 879510; // fyber
	
//	static TJPlacement p;

	private static Supersonic mMediationAgent = null;
	private static MySupersonicListener mySupersonicListener = new MySupersonicListener();
	private static MyNativeXListener myNativeXListener = new MyNativeXListener();

	class AdType {
		public static final int AdFyber = 0;
		public static final int AdSupersonic = 1;
		public static final int AdNativeX = 2;
		public static final int AdAdxmi = 3;
		public static final int AdTapjoy = 4;
	};

	static void initAd(final int type, final String uId, final String appkey, final String token) {
		final Context context = Cocos2dxActivity.getContext();
		final Activity activity = Cocos2dxActivity.getContext();

		Cocos2dxActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
				case AdType.AdFyber:
					SponsorPay.start(appkey, uId, token, activity);
					break;
				case AdType.AdSupersonic:
					mMediationAgent = SupersonicFactory.getInstance();
					mMediationAgent.setOfferwallListener(mySupersonicListener);
					mMediationAgent.initOfferwall(activity, appkey, uId);
					mMediationAgent.initRewardedVideo(activity, appkey, uId);
					break;
				case AdType.AdNativeX:
					_nativeXStep.put(K_NATIVEX_PLACEMENT_OFFER, NX_FREE);
					_nativeXStep.put(K_NATIVEX_PLACEMENT_VIDEO, NX_FREE);

					MonetizationManager.enableLogging(RR.debug());
					MonetizationManager.createSession(context, appkey, uId, myNativeXListener);
					break;
				case AdType.AdAdxmi:
					AdManager.getInstance(context).setEnableDebugLog(RR.debug());
					AdManager.getInstance(context).init(appkey, token);

					OffersManager.getInstance(context).setCustomUserId(uId);
					VideoAdManager.getInstance(context).setCustomUserId(uId);
					OffersManager.getInstance(context).onAppLaunch();			
					// init video
					VideoAdManager.getInstance(context).requestVideoAd(new VideoAdRequestListener() {

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
//					Hashtable<String, Object> connectFlags = new Hashtable<String, Object>();
//					connectFlags.put(TapjoyConnectFlag.ENABLE_LOGGING, RR.debug() ? "true" : "false");
//					connectFlags.put(TapjoyConnectFlag.STORE_NAME, "Google Play");
//					connectFlags.put(TapjoyConnectFlag.USER_ID, uId);
//					
//					Tapjoy.setDebugEnabled(RR.debug());
//					
//					Tapjoy.connect(context, appkey, connectFlags, new TJConnectListener() {
//						
//						@Override
//						public void onConnectSuccess() {
////							TJPlacementListener placementListener = this;
//							
//							p = new TJPlacement(context, "app_launch", new TJPlacementListener() {
//								
//								@Override
//								public void onRewardRequest(TJPlacement arg0, TJActionRequest arg1, String arg2, int arg3) {
//									// TODO Auto-generated method stub
//									
//								}
//								
//								@Override
//								public void onRequestSuccess(TJPlacement arg0) {
//									// TODO Auto-generated method stub
//									
//								}
//								
//								@Override
//								public void onRequestFailure(TJPlacement arg0, TJError arg1) {
//									// TODO Auto-generated method stub
//									
//								}
//								
//								@Override
//								public void onPurchaseRequest(TJPlacement arg0, TJActionRequest arg1, String arg2) {
//									// TODO Auto-generated method stub
//									
//								}
//								
//								@Override
//								public void onContentShow(TJPlacement arg0) {
//									// TODO Auto-generated method stub
//									
//								}
//								
//								@Override
//								public void onContentReady(TJPlacement arg0) {
//									// TODO Auto-generated method stub
//									p.showContent();
//								}
//								
//								@Override
//								public void onContentDismiss(TJPlacement arg0) {
//									// TODO Auto-generated method stub
//									
//								}
//							});
//							
//							p.requestContent();
//						}
//						
//						@Override
//						public void onConnectFailure() {
//							
//						}
//					});
					
					break;
				default:
					assert(false);
					break;
				}
			}
		});
	}

	static void callOfferwall(final int type) {
		final Context context = Cocos2dxActivity.getContext();

		Cocos2dxActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
				case AdType.AdFyber:
					Cocos2dxActivity.getContext().startActivityForResult(
							SponsorPayPublisher.getIntentForOfferWallActivity(Cocos2dxActivity.getContext(), true),
							OFFERWALL_REQUEST_CODE);
					break;
				case AdType.AdSupersonic:
					if (mMediationAgent != null && mMediationAgent.isOfferwallAvailable()) {
						mMediationAgent.showOfferwall();
					}
					break;
				case AdType.AdNativeX:
					_nativeXStep.put(K_NATIVEX_PLACEMENT_OFFER, NX_READY);

					if (MonetizationManager.isAdReady(K_NATIVEX_PLACEMENT_OFFER)) {
						_nativeXStep.put(K_NATIVEX_PLACEMENT_OFFER, NX_FREE);
						MonetizationManager.showAd(Cocos2dxActivity.getContext(), K_NATIVEX_PLACEMENT_OFFER,
								myNativeXListener);

					} else {
						MonetizationManager.fetchAd(Cocos2dxActivity.getContext(), K_NATIVEX_PLACEMENT_OFFER,
								myNativeXListener);
					}
					break;
				case AdType.AdAdxmi:
					OffersManager.getInstance(context).showOffersWall();
					break;
				case AdType.AdTapjoy:
//					p.showContent();
//					if(p.isContentReady()) {
//					    p.showContent();
//					}
//					else {
//					    //handle situation where there is no content to show, or it has not yet downloaded.
//					}
					break;
				default:
					assert(false);
					break;
				}
			}
		});
	}

	static void playVideo(final int type) {
		final Context context = Cocos2dxActivity.getContext();

		Cocos2dxActivity.getContext().runOnUiThread(new Runnable() {

			@Override
			public void run() {
				switch (type) {
				case AdType.AdFyber:
					break;
				case AdType.AdSupersonic:
					if (mMediationAgent != null && mMediationAgent.isRewardedVideoAvailable()) {
						mMediationAgent.showRewardedVideo();
					}
					break;
				case AdType.AdNativeX:
					_nativeXStep.put(K_NATIVEX_PLACEMENT_VIDEO, NX_READY);

					if (MonetizationManager.isAdReady(K_NATIVEX_PLACEMENT_VIDEO)) {
						_nativeXStep.put(K_NATIVEX_PLACEMENT_VIDEO, NX_FREE);
						MonetizationManager.showAd(Cocos2dxActivity.getContext(), K_NATIVEX_PLACEMENT_VIDEO,
								myNativeXListener);

					} else {
						MonetizationManager.fetchAd(Cocos2dxActivity.getContext(), K_NATIVEX_PLACEMENT_VIDEO,
								myNativeXListener);
					}
					break;
				case AdType.AdAdxmi:
					VideoAdManager.getInstance(context).showVideo(context, new VideoAdListener() {

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
				default:
					assert(false);
					break;
				}
			}
		});
	}

	public static void onResume(Activity ctx) {

		if (mMediationAgent != null) {

			mMediationAgent.onResume(ctx);
//			Tapjoy.onActivityStart(ctx);
		}
	}

	public static void onPause(Activity ctx) {

		if (mMediationAgent != null) {
			mMediationAgent.onPause(ctx);
		}
	}
	
	public static void onStop(Activity ctx) {
//		Tapjoy.onActivityStop(ctx);
	}
}
