package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.HashMap;

import com.google.gwt.xml.client.Node;

public class SetupUpdater extends Updater {
	private Node firstSetting;
	private final HashMap<String, DataModelEntityBase> xmlNodeNameToSetting = createSettingMap();

	public SetupUpdater(Node setting) {
		firstSetting = setting;
	}

	private HashMap<String, DataModelEntityBase> createSettingMap() {
		HashMap<String, DataModelEntityBase> m = new HashMap<String, DataModelEntityBase>();
		m.put("AftertouchCurve", Setup.get().systemSettings.aftertouchCurve);
		m.put("AutoLoadSelectedPreset", Setup.get().systemSettings.autoLoad);
		m.put("BaseUnitUIMode", Setup.get().systemSettings.baseUnitUIMode);
		m.put("BenderCurve", Setup.get().systemSettings.benderCurve);
		m.put("DateTimeAdjustment", Setup.get().systemSettings.datetimeAdjustment);
		m.put("DebugLevel", Setup.get().systemSettings.debugLevel);
		m.put("DeviceName", Setup.get().systemSettings.deviceName);
		m.put("EditSmoothingTime", Setup.get().systemSettings.editSmoothingTime);
		m.put("EncoderAcceleration", Setup.get().systemSettings.encoderAcceleration);
		m.put("IndicateBlockedUI", Setup.get().systemSettings.indicateBlockedUI);
		m.put("KioskMode", Setup.get().systemSettings.kioskMode);
		m.put("NoteShift", Setup.get().systemSettings.noteShift);
		m.put("ParameterEditModeRibbonBehaviour", Setup.get().systemSettings.editmodeRibbonBehavior);
		m.put("Passphrase", Setup.get().systemSettings.passPhrase);
		m.put("Pedal1Type", Setup.get().systemSettings.pedal1Type);
		m.put("Pedal2Type", Setup.get().systemSettings.pedal2Type);
		m.put("Pedal3Type", Setup.get().systemSettings.pedal3Type);
		m.put("Pedal4Type", Setup.get().systemSettings.pedal4Type);
		m.put("PitchbendOnPressedKeys", Setup.get().systemSettings.benderOnPressedKeys);
		m.put("PresetDragDropEnabled", Setup.get().systemSettings.presetDragEnabled);
		m.put("PresetGlitchSuppression", Setup.get().systemSettings.presetGlitchSuppression);
		m.put("PresetStoreModeSetting", Setup.get().systemSettings.presetStoreModeSetting);
		m.put("RandomizeAmount", Setup.get().systemSettings.randomizeAmount);
		m.put("RibbonRelFactor", Setup.get().systemSettings.ribbonRelativeFactor);
		m.put("SendPresetAsLPCWriteFallback", Setup.get().systemSettings.sendPresetAsLPCFallback);
		m.put("SignalFlowIndication", Setup.get().systemSettings.signalFlowIndication);
		m.put("SSID", Setup.get().systemSettings.ssid);
		m.put("TransitionTime", Setup.get().systemSettings.transitionTime);
		m.put("VelocityCurve", Setup.get().systemSettings.velocityCurve);
		m.put("RandomizeAmount", Setup.get().systemSettings.randomizeAmount);

		return m;
	}

	public void doUpdate() {
		Node setting = firstSetting;

		while (setting != null) {
			if (setting.getNodeType() == Node.ELEMENT_NODE) {
				Node valueNode = getChild(setting, "value");

				if (didChange(valueNode)) {
					DataModelEntityBase s = findSettingFromTagName(setting.getNodeName());
					s.fromString(getText(valueNode));

					if (s instanceof DoubleDataModelEntity) {
						ValueUpdater u = new ValueUpdater((ValueDataModelEntity) s);
						u.update(setting);
					}
				}
			}
			setting = setting.getNextSibling();
		}

	}

	private DataModelEntityBase findSettingFromTagName(String nodeName) {
		return xmlNodeNameToSetting.get(nodeName);
	}
}
