package com.nonlinearlabs.NonMaps.client.world.overlay.html.presetSearch;

import com.google.gwt.core.client.GWT;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.CheckBox;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.TextArea;
import com.nonlinearlabs.NonMaps.client.presenters.PresetSearchProvider;

public class PresetSearch extends Composite {
	interface SetupUiBinder extends UiBinder<HTMLPanel, PresetSearch> {
	}

	private static SetupUiBinder ourUiBinder = GWT.create(SetupUiBinder.class);

	@UiField
	RadioButton logicalAnd, logicalOr;

	@UiField
	Button resetQuery, prev, next;

	@UiField
	TextArea search;

	@UiField
	CheckBox searchCriteriaName, searchCriteriaComment, searchCriteriaDeviceName, zoomToAllMatches;

	@UiField
	Label numMatches;

	public PresetSearch() {
		initWidget(ourUiBinder.createAndBindUi(this));

		connectEventHandlers();
		connectUpdate();
	}

	public void connectEventHandlers() {
		search.addKeyUpHandler(b -> getUseCases().setQuery(search.getText()));

		logicalAnd.addValueChangeHandler(b -> getUseCases().setCombinationAnd());
		logicalOr.addValueChangeHandler(b -> getUseCases().setCombinationOr());
		resetQuery.addClickHandler(b -> getUseCases().resetQuery());

		searchCriteriaName.addClickHandler(b -> getUseCases().toggleSearchInNames());
		searchCriteriaComment.addClickHandler(b -> getUseCases().toggleSearchInComments());
		searchCriteriaDeviceName.addClickHandler(b -> getUseCases().toggleSearchInDeviceNames());

		zoomToAllMatches.addClickHandler(b -> getUseCases().toggleZoomToAllMatches());

		prev.addClickHandler(b -> getUseCases().highlightPrev());
		next.addClickHandler(b -> getUseCases().highlightNext());
	}

	public com.nonlinearlabs.NonMaps.client.useCases.PresetSearch getUseCases() {
		return com.nonlinearlabs.NonMaps.client.useCases.PresetSearch.get();
	}

	public void connectUpdate() {
		PresetSearchProvider.get().register(p -> applyPresenter(p));
	}

	protected boolean applyPresenter(com.nonlinearlabs.NonMaps.client.presenters.PresetSearch t) {
		search.setText(t.query);
		logicalAnd.setValue(t.andCombination);
		logicalOr.setValue(t.orCombination);
		searchCriteriaName.setValue(t.searchInNames);
		searchCriteriaComment.setValue(t.searchInComments);
		searchCriteriaDeviceName.setValue(t.searchInDeviceNames);
		zoomToAllMatches.setValue(t.zoomToAllMatches);
		numMatches.setText(t.numMatches);
		return isAttached();
	}

}
