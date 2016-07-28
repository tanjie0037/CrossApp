
package org.CrossApp.lib;

import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

public class CrossAppAccelerometer implements SensorEventListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = CrossAppAccelerometer.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================

	private final Context mContext;
	private final SensorManager mSensorManager;
	private final Sensor mAccelerometer;
	private final int mNaturalOrientation;

	private long lastUpdateTime;
	private static final int UPTATE_INTERVAL_TIME = 1000;
	private float mInterval;
	
	// ===========================================================
	// Constructors
	// ===========================================================

	public CrossAppAccelerometer(final Context pContext) {
		this.mContext = pContext;

		this.mSensorManager = (SensorManager) this.mContext.getSystemService(Context.SENSOR_SERVICE);
		this.mAccelerometer = this.mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

		final Display display = ((WindowManager) this.mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		this.mNaturalOrientation = display.getOrientation();
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public void enable() {
		this.mSensorManager.registerListener(this, this.mAccelerometer, SensorManager.SENSOR_DELAY_GAME);
	}

    public void setInterval(float interval) 
    {
    	mInterval = interval;
    	
        // Honeycomb version is 11
        if(android.os.Build.VERSION.SDK_INT < 11) 
        {
	    this.mSensorManager.registerListener(this, this.mAccelerometer, SensorManager.SENSOR_DELAY_GAME);
        } 
        else
        {
	    //convert seconds to microseconds
	    this.mSensorManager.registerListener(this, this.mAccelerometer, (int)(interval*100000));
        }
	}
      
	public void disable() {
		this.mSensorManager.unregisterListener(this);
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onSensorChanged(final SensorEvent pSensorEvent) {
		if (pSensorEvent.sensor.getType() != Sensor.TYPE_ACCELEROMETER) {
			return;
		}

		// The current time
		long currentUpdateTime = System.currentTimeMillis();
		
		// Two detection time interval
		long timeInterval = currentUpdateTime - lastUpdateTime;
		
		// Determine whether detection time interval has been reached
		if (timeInterval < UPTATE_INTERVAL_TIME*mInterval)		
			return;
		
		// The time into the last time
		lastUpdateTime = currentUpdateTime;
				
		float x = pSensorEvent.values[0];
		float y = pSensorEvent.values[1];
		final float z = pSensorEvent.values[2];

		/*
		 * Because the axes are not swapped when the device's screen orientation
		 * changes. So we should swap it here. In tablets such as Motorola Xoom,
		 * the default orientation is landscape, so should consider this.
		 */
		final int orientation = this.mContext.getResources().getConfiguration().orientation;

		if ((orientation == Configuration.ORIENTATION_LANDSCAPE) && (this.mNaturalOrientation != Surface.ROTATION_0)) {
			final float tmp = x;
			x = -y;
			y = tmp;
		} else if ((orientation == Configuration.ORIENTATION_PORTRAIT) && (this.mNaturalOrientation != Surface.ROTATION_0)) {
			final float tmp = x;
			x = y;
			y = -tmp;
		}		
		
		CrossAppGLSurfaceView.queueAccelerometer(x,y,z,mInterval);
		
		/*
		if(BuildConfig.DEBUG) {
			Log.d(TAG, "x = " + pSensorEvent.values[0] + " y = " + pSensorEvent.values[1] + " z = " + pSensorEvent.values[2]);
		}
		*/
	}

	@Override
	public void onAccuracyChanged(final Sensor pSensor, final int pAccuracy) {
	}

	// ===========================================================
	// Methods
        // Native method called from CrossAppGLSurfaceView (To be in the same thread)
	// ===========================================================
    
	public static native void onSensorChanged(final float pX, final float pY, final float pZ, float pTimestamp);

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
