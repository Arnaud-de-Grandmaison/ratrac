#pragma once

#include "ratrac/Matrix.h"
#include "ratrac/Tuple.h"

namespace ratrac {
class Transformable {
public:
  Transformable()
      : m_transform(Matrix::identity()),
        m_inverted_transform(Matrix::identity()) {}
  Transformable(const Matrix &M)
      : m_transform(M), m_inverted_transform(inverse(M)) {}
  Transformable(Matrix &&M)
      : m_transform(std::move(M)), m_inverted_transform(inverse(m_transform)) {}
  Transformable(const Transformable &other)
      : m_transform(other.m_transform),
        m_inverted_transform(other.m_inverted_transform) {}
  Transformable(Transformable &&other)
      : m_transform(std::move(other.m_transform)),
        m_inverted_transform(std::move(other.m_inverted_transform)) {}

  Transformable &operator=(const Transformable &rhs) {
    m_transform = rhs.m_transform;
    m_inverted_transform = rhs.m_inverted_transform;
    return *this;
  }
  Transformable &operator=(Transformable &&rhs) {
    m_transform = std::move(rhs.m_transform);
    m_inverted_transform = std::move(rhs.m_inverted_transform);
    return *this;
  }

  bool operator==(const Transformable &rhs) const {
    return m_transform == rhs.m_transform;
  }

  const Matrix &transform() const { return m_transform; }
  const Matrix &inverse_transform() const { return m_inverted_transform; }

  Transformable &transform(const Matrix &M) {
    m_transform = M;
    recompute();
    update();
    return *this;
  }

  Transformable &transform(Matrix &&M) {
    m_transform = std::move(M);
    recompute();
    update();
    return *this;
  }

  Tuple transform(const Tuple &point) const { return m_transform * point; }
  Tuple inverse_transform(const Tuple &point) const {
    return m_inverted_transform * point;
  }

  virtual void update() {}

private:
  Matrix m_transform;
  Matrix m_inverted_transform;

  void recompute() { m_inverted_transform = inverse(m_transform); }
};

} // namespace ratrac