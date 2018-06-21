package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

abstract class ValueControl extends ZoomReactingControl {

	ValueControl(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(getValue());
	}

	protected boolean isBiPolar() {
		return getParentParameterLayout().getPresenter().bipolar;
	}

	public double getValue() {
		return getParentParameterLayout().getPresenter().valueRange.value;
	}

	public String getDisplayValue() {
		return getParentParameterLayout().getPresenter().displayValue;
	}

	public Parameter getParentParameterLayout() {
		Control parent = getParent();

		while (parent != null) {
			if (parent instanceof Parameter) {
				return (Parameter) parent;
			}

			parent = parent.getParent();
		}

		return null;
	}
}
