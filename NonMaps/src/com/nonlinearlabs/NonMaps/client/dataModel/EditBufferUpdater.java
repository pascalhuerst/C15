package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;

public class EditBufferUpdater extends Updater {
	private Node rootNode;

	public EditBufferUpdater(Node editBufferNode) {
		rootNode = editBufferNode;
	}

	public void doUpdate() {
		processChangedChildrenElements(rootNode, "parameter-group", child -> processGroup(child));
	}

	private void processGroup(Node c) {
		String groupId = getAttributeValue(c, "id");
		ParameterGroupModel target = EditBufferModel.get().getGroup(groupId);
		ParameterGroupUpdater updater = new ParameterGroupUpdater(c, target);
		updater.doUpdate();
	}

}
