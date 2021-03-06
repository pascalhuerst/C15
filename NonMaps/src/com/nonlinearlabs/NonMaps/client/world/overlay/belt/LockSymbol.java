package com.nonlinearlabs.NonMaps.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class LockSymbol extends Label {

    public LockSymbol(OverlayLayout parent) {
        super(parent);
        setFontHeightInMM(getLockSizeInMM());
    }

    @Override
    public String getDrawText(Context2d ctx) {
        if (NonMaps.get().getNonLinearWorld().getParameterEditor().isAnyParameterLocked())
            return "\ue20a";
        return "";
    }

    public double getLockSizeInMM() {
        return 7;
    }
}