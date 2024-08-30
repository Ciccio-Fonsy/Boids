#ifndef VEC3
#define VEC3

#include <cmath>
#include <stdexcept>

class Vec3
{ // vettore tridimensionale
 public:
  double x, y, z;
  Vec3()
      : x(0)
      , y(0)
      , z(0)
  {} // costruttore vuoto
  Vec3(double x_, double y_, double z_)
      : x(x_)
      , y(y_)
      , z(z_)
  {} // costruttore

  // uguaglianza
  bool operator==(const Vec3& other) const
  {
    return x == other.x && y == other.y && z == other.z;
  }

  // disuguaglianza
  bool operator!=(const Vec3& other) const
  {
    return !(*this == other);
  }

  // somma
  Vec3 operator+(const Vec3& other) const
  {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }

  // somma e assegna
  Vec3 operator+=(const Vec3& other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  // differenza
  Vec3 operator-(const Vec3& other) const
  {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }

  // differenza e assegna
  Vec3 operator-=(const Vec3& other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  // prodotto per uno scalare
  Vec3 operator*(double scalar) const
  {
    return Vec3(x * scalar, y * scalar, z * scalar);
  }

  // prodotto per uno scalare e assegna
  Vec3 operator*=(double scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  // divisione per uno scalare
  Vec3 operator/(double scalar) const
  {
    return Vec3(x / scalar, y / scalar, z / scalar);
  }

  // divisione per uno scalare e assegna
  Vec3 operator/=(double scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  // norma
  double norm() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  // normalizza
  Vec3 normalize() const
  {
    double n = norm();
    if (n != 0) {
      return Vec3(x / n, y / n, z / n);
    } else {
      return *this;
    }
  }

  // prodotto scalare
  double dot(const Vec3& other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }

  // prodotto vettoriale
  Vec3 cross(const Vec3& other) const
  {
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
  }

  // accesso agli elementi
  double& operator[](int i)
  {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    if (i == 2)
      return z;
    throw std::out_of_range("index out of range");
  }

  // accesso agli elementi costante
  const double& operator[](int i) const
  {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    if (i == 2)
      return z;
    throw std::out_of_range("index out of range");
  }

  // restituisci per stampa
  std::string to_string() const
  {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", "
         + std::to_string(z) + ")";
  }
};

#endif