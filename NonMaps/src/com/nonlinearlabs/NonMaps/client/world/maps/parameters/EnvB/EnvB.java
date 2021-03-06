package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class EnvB extends ParameterGroupVertical {

	public EnvB(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, getName()));
		addChild(new EnvBControls(this));
	}

	@Override
	public RGB getColorSliderHighlight() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	@Override
	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	@Override
	public RGB getColorModuleBackground() {
		return ColorTable.getColorEnvOscBBackground();
	}

	@Override
	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorEnvOscBSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

	@Override
	public String getID() {
		return "Env B";
	}
}
