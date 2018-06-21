package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;

public class ParameterName extends LabelLarge {

	public ParameterName(ModulatableParameter p) {
		super(p);
	}

	public ParameterName(ModulatableParameter p, String name) {
		super(p, name);
	}

	@Override
	public ModulatableParameter getParent() {
		return (ModulatableParameter) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), 0, getNameBackgroundColor(), null);
		super.draw(ctx, invalidationMask);
	}

	private RGB getNameBackgroundColor() {
		return getParent().getPresenter().selected ? getColorObjectContourSelected() : new Gray(87);
	}

	private int getBackgroundRoundings() {
		return getParent().getBackgroundRoundings() & Rect.ROUNDING_TOP;
	}

	@Override
	public RGB getColorFont() {
		return getParent().getPresenter().selected ? getColorSliderBackground() : super.getColorFont();
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	protected String getDisplayText() {
		Name n = getName();
		if (n != null)
			return getName().getShortName();

		return super.getDisplayText();
	}

}
