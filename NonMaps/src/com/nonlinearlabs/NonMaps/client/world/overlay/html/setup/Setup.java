package com.nonlinearlabs.NonMaps.client.world.overlay.html.setup;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.DivElement;
import com.google.gwt.dom.client.InputElement;
import com.google.gwt.dom.client.Style.Display;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.ListBox;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.TextArea;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.AftertouchCurve;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BenderCurve;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.DisplayScaling;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.EditParameter;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.PedalType;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.SelectionAutoScroll;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.StripeBrightness;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.VelocityCurve;
import com.nonlinearlabs.NonMaps.client.presenters.DeviceInformation;
import com.nonlinearlabs.NonMaps.client.presenters.DeviceInformationProvider;
import com.nonlinearlabs.NonMaps.client.presenters.DeviceSettings;
import com.nonlinearlabs.NonMaps.client.presenters.DeviceSettingsProvider;
import com.nonlinearlabs.NonMaps.client.presenters.LocalSettings;
import com.nonlinearlabs.NonMaps.client.presenters.LocalSettingsProvider;
import com.nonlinearlabs.NonMaps.client.useCases.EditBuffer;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.html.Range;

public class Setup extends Composite {
	interface SetupUiBinder extends UiBinder<HTMLPanel, Setup> {
	}

	private static SetupUiBinder ourUiBinder = GWT.create(SetupUiBinder.class);

	@UiField
	Button deviceSettingsButton, uiSettingsButton, systemInfoButton, aboutButton;

	@UiField
	DivElement deviceSettings, uiSettings, systemInfo, about;

	@UiField
	ListBox velocityCurve, aftertouchCurve, benderCurve, pedal1Type, pedal2Type, pedal3Type, pedal4Type, selectionAutoScroll,
			editParameter, scalingFactor, stripeBrightness;

	@UiField
	Label pedal1DisplayString, pedal2DisplayString, pedal3DisplayString, pedal4DisplayString, editSmoothingTimeDisplayString, freeMemory,
			uiVersion, rtVersion, osVersion, dateTime;

	@UiField
	InputElement pedal1Slider, pedal2Slider, pedal3Slider, pedal4Slider;

	@UiField
	RadioButton presetGlitchSurpressionOn, presetGlitchSurpressionOff, showContextMenusOn, showContextMenusOff, presetDragDropOn,
			presetDragDropOff, bitmapCacheOn, bitmapCacheOff, developerOptionsOn, developerOptionsOff;

	@UiField
	InputElement editSmoothingTimeSlider;

	@UiField
	TextArea deviceName;

	@UiField
	Button saveDeviceName;

	Range editSmoothingTimeRange;
	Range pedal1Range, pedal2Range, pedal3Range, pedal4Range;

	public Setup() {
		initWidget(ourUiBinder.createAndBindUi(this));

		pedal1Range = Range.wrap(pedal1Slider);
		pedal2Range = Range.wrap(pedal2Slider);
		pedal3Range = Range.wrap(pedal3Slider);
		pedal4Range = Range.wrap(pedal4Slider);

		setupTexts();
		connectEventHandlers();
		connectUpdate();

		switchPage(deviceSettingsButton, deviceSettings);
	}

	public void setupTexts() {
		fillListboxWithOptions(velocityCurve, DeviceSettings.VelocityCurve.options);
		fillListboxWithOptions(aftertouchCurve, DeviceSettings.AftertouchCurve.options);
		fillListboxWithOptions(benderCurve, DeviceSettings.BenderCurve.options);
		fillListboxWithOptions(pedal1Type, DeviceSettings.Pedal.options);
		fillListboxWithOptions(pedal2Type, DeviceSettings.Pedal.options);
		fillListboxWithOptions(pedal3Type, DeviceSettings.Pedal.options);
		fillListboxWithOptions(pedal4Type, DeviceSettings.Pedal.options);
		fillListboxWithOptions(selectionAutoScroll, LocalSettings.SelectionAutoScroll.options);
		fillListboxWithOptions(editParameter, LocalSettings.EditParameter.options);
		fillListboxWithOptions(scalingFactor, LocalSettings.DisplayScalingFactor.options);
		fillListboxWithOptions(stripeBrightness, LocalSettings.StripeBrightness.options);

		fillRadioButtons(presetGlitchSurpressionOn, presetGlitchSurpressionOff, DeviceSettings.PresetGlitchSurpression.options);
		fillRadioButtons(showContextMenusOn, showContextMenusOff, LocalSettings.ContextMenus.options);
		fillRadioButtons(presetDragDropOn, presetDragDropOff, LocalSettings.PresetDragDrop.options);
		fillRadioButtons(bitmapCacheOn, bitmapCacheOff, LocalSettings.BitmapCache.options);
		fillRadioButtons(developerOptionsOn, developerOptionsOff, LocalSettings.ShowDeveloperOptions.options);
	}

	public void connectEventHandlers() {
		SystemSettings settings = SystemSettings.get();
		com.nonlinearlabs.NonMaps.client.useCases.LocalSettings locals = com.nonlinearlabs.NonMaps.client.useCases.LocalSettings.get();

		deviceSettingsButton.addClickHandler(e -> switchPage(deviceSettingsButton, deviceSettings));
		uiSettingsButton.addClickHandler(e -> switchPage(uiSettingsButton, uiSettings));
		systemInfoButton.addClickHandler(e -> switchPage(systemInfoButton, systemInfo));
		aboutButton.addClickHandler(e -> switchPage(aboutButton, about));

		velocityCurve.addChangeHandler(e -> settings.setVelocityCurve(VelocityCurve.values()[velocityCurve.getSelectedIndex()]));
		aftertouchCurve.addChangeHandler(e -> settings.setAftertouchCurve(AftertouchCurve.values()[aftertouchCurve.getSelectedIndex()]));
		benderCurve.addChangeHandler(e -> settings.setBenderCurve(BenderCurve.values()[benderCurve.getSelectedIndex()]));
		pedal1Type.addChangeHandler(e -> settings.setPedal1Type(PedalType.values()[pedal1Type.getSelectedIndex()]));
		pedal2Type.addChangeHandler(e -> settings.setPedal2Type(PedalType.values()[pedal2Type.getSelectedIndex()]));
		pedal3Type.addChangeHandler(e -> settings.setPedal3Type(PedalType.values()[pedal3Type.getSelectedIndex()]));
		pedal4Type.addChangeHandler(e -> settings.setPedal4Type(PedalType.values()[pedal4Type.getSelectedIndex()]));
		selectionAutoScroll.addChangeHandler(e -> locals.setSelectionAutoScroll(SelectionAutoScroll.values()[selectionAutoScroll
				.getSelectedIndex()]));
		editParameter.addChangeHandler(e -> locals.setEditParameter(EditParameter.values()[editParameter.getSelectedIndex()]));
		scalingFactor.addChangeHandler(e -> locals.setDisplayScaling(DisplayScaling.values()[scalingFactor.getSelectedIndex()]));
		stripeBrightness.addChangeHandler(e -> locals.setStripeBrightness(StripeBrightness.values()[stripeBrightness.getSelectedIndex()]));

		presetGlitchSurpressionOn.addClickHandler(e -> settings.setPresetGlitchSurpression(BooleanValues.on));
		presetGlitchSurpressionOff.addClickHandler(e -> settings.setPresetGlitchSurpression(BooleanValues.off));

		showContextMenusOn.addClickHandler(e -> locals.setContextMenus(BooleanValues.on));
		showContextMenusOff.addClickHandler(e -> locals.setContextMenus(BooleanValues.off));

		presetDragDropOn.addClickHandler(e -> locals.setPresetDragDrop(BooleanValues.on));
		presetDragDropOff.addClickHandler(e -> locals.setPresetDragDrop(BooleanValues.off));

		bitmapCacheOn.addClickHandler(e -> locals.setBitmapCache(BooleanValues.on));
		bitmapCacheOff.addClickHandler(e -> locals.setBitmapCache(BooleanValues.off));

		developerOptionsOn.addClickHandler(e -> locals.showDeveloperOptions(BooleanValues.on));
		developerOptionsOff.addClickHandler(e -> locals.showDeveloperOptions(BooleanValues.off));

		editSmoothingTimeRange = Range.wrap(editSmoothingTimeSlider);
		editSmoothingTimeRange.addValueChangeHandler(e -> settings.setEditSmoothingTime(editSmoothingTimeRange.getValue().doubleValue()));

		pedal1Range.addValueChangeHandler(e -> EditBuffer.get().setParameterValue("CS", 254, e.getValue().doubleValue(), true));
		pedal2Range.addValueChangeHandler(e -> EditBuffer.get().setParameterValue("CS", 259, e.getValue().doubleValue(), true));
		pedal3Range.addValueChangeHandler(e -> EditBuffer.get().setParameterValue("CS", 264, e.getValue().doubleValue(), true));
		pedal4Range.addValueChangeHandler(e -> EditBuffer.get().setParameterValue("CS", 269, e.getValue().doubleValue(), true));

		saveDeviceName.addClickHandler(e -> settings.setDeviceName(deviceName.getValue()));
	}

	public void connectUpdate() {
		DeviceSettingsProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});

		LocalSettingsProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});

		DeviceInformationProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});
	}

	private void fillListboxWithOptions(ListBox box, String[] options) {
		for (String s : options)
			box.addItem(s);
	}

	private void fillRadioButtons(RadioButton on, RadioButton off, String[] options) {
		on.setText(options[0]);
		off.setText(options[1]);
	}

	protected void applyPresenter(DeviceSettings t) {
		velocityCurve.setSelectedIndex(t.velocityCurve.selected);
		aftertouchCurve.setSelectedIndex(t.aftertouchCurve.selected);
		benderCurve.setSelectedIndex(t.benderCurve.selected);

		applyPedalValues(t.pedal1, pedal1Type, pedal1Range, pedal1DisplayString);
		applyPedalValues(t.pedal2, pedal2Type, pedal2Range, pedal2DisplayString);
		applyPedalValues(t.pedal3, pedal3Type, pedal3Range, pedal3DisplayString);
		applyPedalValues(t.pedal4, pedal4Type, pedal4Range, pedal4DisplayString);

		presetGlitchSurpressionOn.setValue(t.presetGlitchSurpession.value);
		presetGlitchSurpressionOff.setValue(!t.presetGlitchSurpession.value);
		editSmoothingTimeRange.setValue(t.editSmoothingTime.sliderPosition);
		editSmoothingTimeDisplayString.setText(t.editSmoothingTime.displayValue);

		deviceName.setText(t.deviceName);
	}

	public void applyPedalValues(DeviceSettings.Pedal src, ListBox type, Range slider, Label text) {
		type.setSelectedIndex(src.selected);
		slider.setValue(src.sliderPosition);
		text.setText(src.displayValue);
	}

	private void applyPresenter(LocalSettings t) {
		selectionAutoScroll.setSelectedIndex(t.selectionAutoScroll.selected);
		editParameter.setSelectedIndex(t.editParameter.selected);
		showContextMenusOn.setValue(t.contextMenus.value);
		showContextMenusOff.setValue(!t.contextMenus.value);
		presetDragDropOn.setValue(t.presetDragDrop.value);
		presetDragDropOff.setValue(!t.presetDragDrop.value);
		scalingFactor.setSelectedIndex(t.displayScalingFactor.selected);
		stripeBrightness.setSelectedIndex(t.stripeBrightness.selected);
		bitmapCacheOn.setValue(t.bitmapCache.value);
		bitmapCacheOff.setValue(!t.bitmapCache.value);
		developerOptionsOn.setValue(t.showDeveloperOptions.value);
		developerOptionsOff.setValue(!t.showDeveloperOptions.value);
	}

	private void applyPresenter(DeviceInformation t) {
		freeMemory.setText(t.freeDiscSpace);
		uiVersion.setText(t.uiVersion);
		rtVersion.setText(t.rtVersion);
		osVersion.setText(t.osVersion);
		dateTime.setText(t.dateTime);
	}

	public void switchPage(Button btn, DivElement page) {
		deviceSettings.getStyle().setDisplay(Display.NONE);
		uiSettings.getStyle().setDisplay(Display.NONE);
		systemInfo.getStyle().setDisplay(Display.NONE);
		about.getStyle().setDisplay(Display.NONE);

		deviceSettingsButton.getElement().removeClassName("active");
		uiSettingsButton.getElement().removeClassName("active");
		systemInfoButton.getElement().removeClassName("active");
		aboutButton.getElement().removeClassName("active");

		btn.getElement().addClassName("active");
		page.getStyle().setDisplay(Display.BLOCK);
	}
}
