package com.nonlinearlabs.NonMaps.client.dataModel.value;

import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;

class ClippedValue extends Value {

	public double getClippedValue() {
		return clip(getRawValue());
	}

	public double clip(double v) {
		return Math.min(getUpperBorder(), Math.max(v, getLowerBorder()));
	}

	public double getUpperBorder() {
		return 1.0;
	}

	public double getLowerBorder() {
		return isBipolar() ? -1.0 : 0.0;
	}

	@Override
	protected void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
		double oldClippedValue = clip(oldRawValue);
		double newClippedValue = clip(newRawValue);

		if (oldClippedValue != newClippedValue)
			onClippedValueChanged(initiator, oldClippedValue, newClippedValue);

		super.onRawValueChanged(initiator, oldRawValue, newRawValue);
	}

	protected void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	}
}
