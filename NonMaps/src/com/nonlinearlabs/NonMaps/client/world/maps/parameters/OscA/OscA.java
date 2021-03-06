package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscA;

import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class OscA extends ParameterGroupVertical {

	public OscA(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, getName()));
		addChild(new OscAControls(this));
	}

	@Override
	public String getID() {
		return "Osc A";
	}

	public RGB getColorSliderHighlight() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	public RGB getColorModuleBackground() {
		return ColorTable.getColorEnvOscABackground();
	}

	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorEnvOscASelectedBackground();
	}

	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}
}
