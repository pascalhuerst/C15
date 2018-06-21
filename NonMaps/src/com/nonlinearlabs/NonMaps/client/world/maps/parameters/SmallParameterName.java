package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class SmallParameterName extends LabelSmall {
	private Parameter parameter;

	public SmallParameterName(MapsLayout parent) {
		super(parent, "");

		recurseParents(p -> {
			if (p instanceof Parameter) {
				parameter = (Parameter) p;
				return false;
			}

			return true;
		});
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	protected String getDisplayText() {
		return parameter.getPresenter().shortName;
	}
}