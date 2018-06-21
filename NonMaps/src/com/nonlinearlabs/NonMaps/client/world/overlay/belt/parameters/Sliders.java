package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class Sliders extends OverlayLayout {

	OverlayControl upper;
	OverlayControl middle;
	OverlayControl lower;

	public Sliders(BeltParameterLayout parent) {
		super(parent);

		addChild(middle = new ParameterValueSlider(this));
		addChild(upper = new ModulationAmountSlider(this, true));
		addChild(lower = new ModulationAmountSlider(this, false));
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double upperLowerHeight = Millimeter.toPixels(2.5);
		double middleHeight = Millimeter.toPixels(3.5);
		upper.doLayout(0, (h - middleHeight) / 2 - upperLowerHeight, w, upperLowerHeight);
		middle.doLayout(0, (h - middleHeight) / 2, w, middleHeight);
		lower.doLayout(0, (h + middleHeight) / 2, w, upperLowerHeight);
	}

	protected Parameter getParameter() {
		// return
		// getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();
		return null;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		/*-
		if (amount > 0)
			getParameter().inc(fine);
		else if (amount < 0)
			getParameter().dec(fine);
			-*/

		return this;
	}
}
