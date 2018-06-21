package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class LabelLarge extends LabelSmall {

	public LabelLarge(MapsLayout parent, Name name) {
		super(parent, name);
	}

	public LabelLarge(MapsLayout parent, String text) {
		super(parent, text);
	}

	public LabelLarge(MapsLayout parent) {
		super(parent, "");
	}

	@Override
	protected double getFontHeight() {
		return 12;
	}

	@Override
	protected double getBasicWidth() {
		return 70;
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getMinHeight() {
		return 10;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return getParent().getLevelOfDetailForFullVisibility() + 1;
	}
}
