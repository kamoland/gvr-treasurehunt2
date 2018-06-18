package com.kamoland.util;

import android.content.Context;
import android.os.Handler;

import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.sdk.controller.Orientation;

/**
 * Created by kamoi on 2018/06/07.
 */

public class DaydreamController {
    static {
        System.loadLibrary("ddcontroller_jni");
    }
    private long nativeHandle;
    private final GvrLayout gvrLayout;

    private final Handler handler;

    private final float[] quatAry = new float[4];

    private boolean touchBtnPressing;
    private boolean appBtnPressing;

    private final EventListener evListener;
    private final Runnable onTouchButtonClickedAction = new Runnable() {
        @Override
        public void run() {
            evListener.onTouchButtonClicked();
        }
    };
    private final Runnable onAppButtonClickedAction = new Runnable() {
        @Override
        public void run() {
            evListener.onAppButtonClicked();
        }
    };
    private final Runnable onOrientationUpdateAction = new Runnable() {
        @Override
        public void run() {
            evListener.onOrientationUpdate(new Orientation(quatAry[0], quatAry[1], quatAry[2], quatAry[3]));
        }
    };

    public interface EventListener {
        void onTouchButtonClicked();
        void onAppButtonClicked();
        void onOrientationUpdate(Orientation  orientation);
    }

    public DaydreamController(Context context, final EventListener evListener) {
        this.evListener = evListener;
        handler = new Handler();

        gvrLayout = new GvrLayout(context);
        nativeHandle = nativeOnCreate(gvrLayout.getGvrApi().getNativeGvrContext());
    }

    public void onDestroy() {
        gvrLayout.shutdown();

        nativeOnDestroy(nativeHandle);
        nativeHandle = 0;
    }

    public void onPause() {
        nativeOnPause(nativeHandle);
    }

    public void onResume() {
        nativeOnResume(nativeHandle);
    }

    public void update() {
        nativeUpdateController(nativeHandle);

        int allButtonsState = nativeGetAllButtonsState(nativeHandle);

        // タッチボタンの押下状態
        if ((allButtonsState & 0x01) != 0) {
            if (!touchBtnPressing) {
                // ボタンを押下した直後
                touchBtnPressing = true;
                handler.post(onTouchButtonClickedAction);
            }
        } else {
            if (touchBtnPressing) {
                // ボタンを離した直後
                touchBtnPressing = false;
            }
        }
        // Appボタンの押下状態
        if ((allButtonsState & 0x04) != 0) {
            if (!appBtnPressing) {
                // ボタンを押下した直後
                appBtnPressing = true;
                handler.post(onAppButtonClickedAction);
            }
        } else {
            if (appBtnPressing) {
                // ボタンを離した直後
                appBtnPressing = false;
            }
        }
        // コントローラーの方位を取得する(クオータニオン)
        nativeGetControllerOrientation(nativeHandle, quatAry);
        handler.post(onOrientationUpdateAction);
    }

    // コントローラーの方位をYawPitchRollで取得する
    public void getYawPitchRoll(float[] ypr) {
        nativeGetControllerYawPitchRoll(nativeHandle, ypr);
    }

    private native long nativeOnCreate(long gvrContextPtr);
    private native void nativeOnDestroy(long ddControllerJptr);
    private native void nativeOnResume(long ddControllerJptr);
    private native void nativeOnPause(long ddControllerJptr);
    private native void nativeUpdateController(long ddControllerJptr);
    private native void nativeGetControllerOrientation(long ddControllerJptr, float[] quatAry);
    private native void nativeGetControllerYawPitchRoll(long ddControllerJptr, float[] ypr);
    private native int nativeGetAllButtonsState(long ddControllerJptr);
}
