package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Position;

class RightDrag extends Drag implements Gesture.NeedsFocus, Gesture.HasPosition {

	RightDrag(Gesture predecessor, Position from, Position to, boolean fine) {
		super(predecessor, from, to, fine);
	}

	@Override
	public Gesture onRightUp() {
		return new RightUp(this);
	}

	@Override
	public Gesture move(boolean fine) {
		return new RightDrag(this, getPosition(), PointerState.get().getPosition(), fine);
	}

}
