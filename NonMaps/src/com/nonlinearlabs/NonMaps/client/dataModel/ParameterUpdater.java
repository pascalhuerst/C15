package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;

public class ParameterUpdater extends Updater {

	private Node root;
	private BasicParameterModel target;

	public ParameterUpdater(Node c, BasicParameterModel p) {
		root = c;
		target = p;
	}

	public void doUpdate() {
		target.shortName.setValue(getChildText(root,  "short-name"));
		target.longName.setValue(getChildText(root,  "long-name"));
		
		ValueUpdater vu = new ValueUpdater(target.value);
		vu.update(root);
	}

}
