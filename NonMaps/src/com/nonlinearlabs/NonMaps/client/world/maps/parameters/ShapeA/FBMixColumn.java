package com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class FBMixColumn extends ParameterColumn {

	private class FBMix extends ModulatableSlider {

		private FBMix(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 78;
		}
	}

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 80;
		}
	}

	FBMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new FBMix(this));
		addChild(new EnvC(this));
	}
}
