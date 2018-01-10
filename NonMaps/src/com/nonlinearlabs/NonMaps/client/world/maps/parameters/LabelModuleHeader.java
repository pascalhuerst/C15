package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings.MacroControlMappings;

public class LabelModuleHeader extends LabelSmall {

	public LabelModuleHeader(MapsLayout parent, Name name) {
		super(parent, name);
	}

	public LabelModuleHeader(MacroControlMappings parent, String text) {
		super(parent, text);
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		setNonSize(parentsWidthFromFirstPass, getNonPosition().getDimension().getHeight());
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		Rect pixRect = getPixRect();

		pixRect.drawRoundedRect(ctx, Rect.ROUNDING_TOP, toXPixels(6), toXPixels(2), new Gray(87), null);

		super.draw(ctx, invalidationMask);
	}

	@Override
	protected double getFontHeight() {
		return 16;
	}

	@Override
	protected double getBasicWidth() {
		return 70;
	}

	@Override
	protected double getBasicHeight() {
		return 28;
	}

	@Override
	public RGB getColorFont() {
		return new RGBA(super.getColorSliderHighlight(), 0.9);
	}

	@Override
	protected boolean shouldShowTextForLevelOfDetail(double levelOfDetail) {
		return true;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 1;
	}
}
