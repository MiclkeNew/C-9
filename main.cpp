#include <iostream>
#include <memory>
#include <utility> // для std::move
#include <cmath>   // для std::sqrt

class Vector3D {
private:
    std::shared_ptr<double[]> coords; // Динамический массив для хранения координат
    size_t size; // Размер массива

public:
    // Конструктор по умолчанию
    Vector3D() : coords(std::make_shared<double[]>(3)), size(3) {
        coords[0] = 0.0;
        coords[1] = 0.0;
        coords[2] = 0.0;
    }

    // Конструктор с параметрами
    Vector3D(double x, double y, double z) : coords(std::make_shared<double[]>(3)), size(3) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    // Деструктор
    ~Vector3D() = default;

    // Конструктор копирования
    Vector3D(const Vector3D& other) : coords(other.coords), size(other.size) {
        // Нет необходимости копировать массив, так как shared_ptr уже управляет им
    }

    // Оператор присваивания копированием
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {
            coords = other.coords; // shared_ptr автоматически управляет временем жизни
            size = other.size;
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

    // Метод для нормализации вектора
    void normalize() {
        double length = std::sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
        if (length > 0) {
            coords[0] /= length;
            coords[1] /= length;
            coords[2] /= length;
        }
    }

    // Метод для вывода вектора
    void print() const {
        std::cout << "Vector3D(" << getX() << ", " << getY() << ", " << getZ() << ")\n";
    }
};

// Функция, принимающая std::shared_ptr<Vector3D>
void processVector(std::shared_ptr<Vector3D> vec) {
    vec->normalize();
}

int main() {
    auto v1 = std::make_shared<Vector3D>(1.0, 2.0, 3.0);
    v1->print();

    auto v2 = v1; // Копирование с помощью shared_ptr
    v2->print();

    processVector(v1); // Нормализация вектора через shared_ptr
    v1->print();
    v2->print(); // v2 тоже изменился, так как shared_ptr разделяет владение

    auto v3 = std::make_shared<Vector3D>(4.0, 5.0, 6.0);
    v3->print();

    // Проверка корректного освобождения ресурсов
    {
        auto v4 = v3; // Создаем еще один shared_ptr на тот же объект
        v4->print();
    } // v4 выходит из области видимости, но v3 все еще существует

    v3->print(); // Проверяем, что v3 все еще валиден

    return 0;
}
