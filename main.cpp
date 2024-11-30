#include <iostream>
#include <memory>
#include <utility> // для std::move

class Vector3D {
private:
    std::unique_ptr<double[]> coords; // Динамический массив для хранения координат
    size_t size; // Размер массива

public:
    // Конструктор по умолчанию
    Vector3D() : coords(std::make_unique<double[]>(3)), size(3) {
        coords[0] = 0.0;
        coords[1] = 0.0;
        coords[2] = 0.0;
    }

    // Конструктор с параметрами
    Vector3D(double x, double y, double z) : coords(std::make_unique<double[]>(3)), size(3) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    // Деструктор
    ~Vector3D() = default;

    // Конструктор копирования
    Vector3D(const Vector3D& other) : coords(std::make_unique<double[]>(3)), size(other.size) {
        std::copy(other.coords.get(), other.coords.get() + size, coords.get());
    }

    // Оператор присваивания копированием
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {
            coords = std::make_unique<double[]>(other.size);
            size = other.size;
            std::copy(other.coords.get(), other.coords.get() + size, coords.get());
        }
        return *this;
    }

    // Конструктор перемещения
    Vector3D(Vector3D&& other) noexcept : coords(std::move(other.coords)), size(other.size) {
        other.size = 0; // Обнуляем размер у перемещаемого объекта
    }

    // Оператор присваивания перемещением
    Vector3D& operator=(Vector3D&& other) noexcept {
        if (this != &other) {
            coords = std::move(other.coords);
            size = other.size;
            other.size = 0; // Обнуляем размер у перемещаемого объекта
        }
        return *this;
    }

    // Метод для получения координат
    double getX() const { return coords[0]; }
    double getY() const { return coords[1]; }
    double getZ() const { return coords[2]; }

    // Метод для вывода вектора
    void print() const {
        std::cout << "Vector3D(" << getX() << ", " << getY() << ", " << getZ() << ")\n";
    }
};

int main() {
    Vector3D v1(1.0, 2.0, 3.0);
    v1.print();

    Vector3D v2 = v1; // Копирование
    v2.print();

    Vector3D v3 = std::move(v1); // Перемещение
    v3.print();

    v1.print(); // v1 теперь невалиден после перемещения

    return 0;
}