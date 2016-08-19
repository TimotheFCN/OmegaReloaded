#include <assert.h>
#include <stdlib.h>
#include "string_layout.h"

ExpressionLayout::ExpressionLayout() :
  m_baseline(0),
  m_parent(nullptr),
  m_sized(false),
  m_positioned(false),
  m_frame(KDRectZero) {
}

ExpressionLayout::~ExpressionLayout() {
}

KDCoordinate ExpressionLayout::baseline() {
  return m_baseline;
}

KDPoint ExpressionLayout::origin() {
  if (m_parent == nullptr) {
    return absoluteOrigin();
  } else {
    return KDPoint(absoluteOrigin().x() - m_parent->absoluteOrigin().x(),
        absoluteOrigin().y() - m_parent->absoluteOrigin().y());
  }
}

void ExpressionLayout::draw(KDContext * ctx, KDPoint p) {
  int i = 0;
  while (ExpressionLayout * c = child(i++)) {
    c->draw(ctx, p);
  }
  render(ctx, absoluteOrigin().translatedBy(p));
}

KDPoint ExpressionLayout::absoluteOrigin() {
  if (!m_positioned) {
    if (m_parent != nullptr) {
      m_frame.setOrigin(m_parent->absoluteOrigin().translatedBy(m_parent->positionOfChild(this)));
    } else {
      m_frame.setOrigin(KDPointZero);
    }
    m_positioned = true;
  }
  return m_frame.origin();
}

KDSize ExpressionLayout::size() {
  if (!m_sized) {
    m_frame.setSize(computeSize());
    m_sized = true;
  }
  return m_frame.size();
}

void ExpressionLayout::setParent(ExpressionLayout* parent) {
  m_parent = parent;
}
