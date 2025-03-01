#ifndef WINDOW_HH
#define WINDOW_HH

#include <string>

#define FENSTER_HEADER
#include "fenster.h"

namespace pro2 {

/**
 * @enum ModKey
 *
 * Enumerado para las 4 teclas de control: Ctrl, Shift, Alt, y Meta.
 */
enum ModKey { Ctrl = 1, Shift = 2, Alt = 4, Meta = 8 };

/**
 * @enum Colors
 *
 * Enumerado de colores con los colores más simples (los 8 colores de la paleta EGA).
 */
enum Colors {
    black = 0x00000000,
    red = 0x00ff0000,
    green = 0x0000ff00,
    blue = 0x000000ff,
    yellow = 0x00ffff00,
    magenta = 0x00ff00ff,
    cyan = 0x0000ffff,
    white = 0x00ffffff,
};

struct Pt {
    int x, y;
};

/**
 * @class Window
 *
 * La clase `Window` permite abrir ventanas en modo gráfico en Linux, MacOS y Windows. Tiene unos
 * pocos métodos que permiten hacer programas simples que muestran gráficos, como pequeños juegos o
 * editores.
 */
class Window {
    /**
     * @brief La estructura de datos que Fenster necesita para trabajar
     */
    fenster fenster_;

    /**
     * @brief El buffer de pixels que se reserva como zona de pintado
     *
     * Cada pixel tiene 32bits, o 4 bytes, y los 3 bytes de menos peso son los valores (entre 0 y
     * 255) de los canales R, G y B (red, green y blue).
     */
    uint32_t *pixels_;

    /**
     * @brief Tamaño del buffer en bytes
     */
    size_t pixels_size_;

    /**
     * @brief Parámetro de `zoom` para esta ventana
     */
    int zoom_ = 1;

    /**
     * @brief Instance del último fotograma (epoch)
     */
    int64_t last_time_;

    /**
     * @brief Contador de frames (o fotogramas)
     */
    size_t frame_count_ = 0;

   public:
    /**
     * @brief Contruye una ventana con título, anchura y altura.
     *
     * El constructor abre una ventana, y el destructor la cierra.
     *
     * El parámetro `zoom` permite visualizar con más comodidad contenido pixelado. Con `zoom = 1`
     * cada pixel de la ventana se corresponde con un pixel de la pantalla. Con `zoom = 3`, cada
     * píxel de la ventana se convierte en un cuadrado de 3x3 píxeles en la ventana.
     *
     * @param title El título de la ventana (un literal de cadena de caracteres)
     * @param width El ancho de la ventana en píxels.
     * @param height El alto de la ventana en píxels.
     * @param zoom El factor de aumento de cada píxel. (Es opcional, si no hay 4o parámetro toma
     * valor 1)
     */
    Window(const char *title, int width, int height, int zoom = 1);

    /**
     * @brief Destruye una ventana, es decir, cierra la ventana abierta en el constructor.
     */
    ~Window();

    /**
     * @brief Gestiona las tareas necesarias para pasar al siguiente fotograma.
     *
     * En todo programa gráfico es necesario: 1) pintar en una superfície, típicamente en memoria,
     * 2) transferir lo que se ha pintado a la pantalla, 3) procesar eventos ocurridos como presión
     * de teclas o movimiento del ratón y actualizar su estado, y 4) esperar el tiempo que quede
     * hasta el siguiente fotograma (en función de la velocidad de refresco, que suele ser de 60Hz,
     * lo que equivale a 16ms por fotograma).
     *
     * `next_frame` hace todas estas cosas en una sola llamada. Además devuelve `false` cuando se ha
     * clicado el botón de cerrar la ventana (típicamente arriba a la derecha, y con una "x"), de
     * forma que se pueda saber si se debe continuar en un bucle de pintado de fotogramas.
     *
     * El uso típico es el siguiente:
     * ```c++
     * while (window.next_frame()) {
     *     // usar los métodos de detección de teclas o ratón, y set_pixel para pintar...
     * }
     * ```
     * Es decir, hasta que no se cierre la ventana llamamos métodos de la ventana para hacer
     * operaciones que resulten en el pintado de la ventana de cierta manera y `next_frame` se hace
     * cargo del resto.
     *
     * Con respecto al teclado y ratón, `next_frame` recoge todos los eventos (presión y soltado de
     * teclas, clicks y movimiento del ratón) que han ocurrido entre el fotograma anterior y el
     * actual, y con todos ellos actualiza el estado final de cada tecla, botón del ratón y
     * posición. Así pues, el usuario de la clase `Window` tiene acceso al estado exacto de las
     * teclas y el ratón en el instante en que se pasa al fotograma actual, y ese estado se conserva
     * fijo mientras transcurre el tiempo entre el fotograma actual y el siguiente, en el que
     * `next_frame` vuelve a revisar los eventos ocurridos en ese intervalo de tiempo.
     *
     * @returns `true` si el programa debe seguir (NO se ha clicado el botón de cerrar la ventana),
     * `false` en caso contrario.
     *
     */
    bool next_frame();

    /**
     * @brief Rellena la ventana con un color.
     *
     * Este método se puede llamar con un color o bien sin parámetros. Si se llama sin parámetros se
     * toma el `color` por defecto, que es el negro (`Colors::black`). De lo contrario se usa el
     * color indicado.
     *
     * @param color El color a utilizar para pintar. Se puede usar uno de los valores del enumerado
     * `Colors`, como `Colors::red`, o bien poner un entero en hexadecimal, como 0x0084fb, que
     * equivale a los 3 valores RGB (o Red-Green-Blue) que conforman el color. Cualquier "color
     * picker" de la web suele mostrar el color hexadecimal en la notación `#0084fb` (de CSS).
     */
    void clear(int color = 0x00000000);

    /**
     * @brief Devuelve el contador de fotogramas pintados hasta el momento.
     *
     * Equivale a la cantidad de veces que se ha llamado a `next_frame`. Se incrementa en 1 unidad
     * en cada fotograma.
     *
     * Este valor es útil al hacer animaciones, ya que permite saber, de una secuencia de imágenes,
     * cuál habría que usar en cada momento.
     *
     * @returns Un entero sin signo (de tipo `size_t`) que corresponde al contador de fotogramas
     * mostrados desde que la ventana se creó.
     */
    size_t frame_count() const {
        return frame_count_;
    }

    /**
     * @brief Determina si cierta tecla se presionó entre el fotograma anterior y el actual
     *
     * El método `next_frame` recoge todas los eventos de teclado y ratón que han ocurrido desde la
     * llamada anterior a `next_frame` (o desde la creación de la ventana) y mantiene el estado de
     * todas las teclas y botones del ratón fijo durante el fotograma actual. Así pues, el método
     * `is_key_pressed` simplemente consulta ese estado, que se mantiene fijo hasta la siguiente
     * llamada a `next_frame`.
     *
     * @param code El código de la tecla de la que se quiere saber si estaba presionada.
     *
     * @returns `true` cuando la tecla `code` estaba presionada al empezar el fotograma actual.
     */
    bool is_key_pressed(int code) const {
        return code >= 0 && code < 128 && fenster_.keys[code];
    }

    /**
     * @brief Determina si cierta tecla de control se presionó entre el fotograma anterior y el
     * actual
     *
     * Método análogo a `is_key_pressed` pero para las teclas de control siguientes: Ctrl, Alt,
     * Shift y Meta. Hay un enumerado de nombre `ModKey` con las 4 teclas: `ModKey::Ctrl`,
     * `ModKey::Alt`, `ModKey::Shift`, y `ModKey::Meta`.
     *
     * @param key La tecla de la que se quiere consultar el estado.
     *
     * @returns `true` si el estado de la tecla era "presionado" al entrar al fotograma actual.
     */
    bool is_modkey_pressed(ModKey key) {
        return fenster_.mod & uint8_t(key);
    }

    /**
     * @brief Determina si el botón izquierdo del ratón se clicó entre el fotograma anterior y el
     * actual
     *
     * Este método se comporta como `is_key_pressed`, consulta la documentación de `is_key_pressed`
     * para saber cómo opera.
     *
     * @returns `true` si el botón del ratón estaba clicado al principio del fotograma actual.
     */
    bool is_mouse_pressed() {
        return bool(fenster_.mouse);
    }

    /**
     * @brief Devuelve la posición del cursor del ratón.
     *
     * @returns Una tupla de tipo `Pt`, con campos `x` e `y`, que se corresponden con las
     * coordenadas de la posición del ratón.
     */
    Pt mouse_pos() const;

    /**
     * @brief Espera que pase un número `ms` de milisegundos sin hacer nada.Alt
     *
     * En ese intervalo de tiempo el programa estará esperando que el método vuelva de la llamada, y
     * por tanto no ejecutará ninguna instrucción.
     *
     * @param ms Número de milisegundos a esperar.
     */
    void sleep(int ms) const {
        fenster_sleep(ms);
    }

    /**
     * @brief Obtiene el color de un pixel de la ventana.
     *
     * @param xy Coordenadas del pixel de la pantalla del que se quiere saber el color.
     * @returns El color del pixel en las coordenadas indicadas.
     */
    uint32_t get_pixel(Pt xy) {
        return fenster_pixel(&fenster_, xy.x * zoom_, xy.y * zoom_);
    }

    /**
     * @brief Cambia un pixel de la ventana.
     *
     * En realidad, `set_pixel` no cambia la ventana directamente, sinó un "buffer" interno que se
     * vuelca en la pantalla de golpe en el momento de llamar a `next_frame`. Esto es más eficiente
     * y maximiza el tiempo en que el fotograma está inmóvil en la pantalla mostrando una imagen
     * fija, ya que el pintado podría llevar tanto tiempo que los fotogramas no se verían completos
     * en la pantalla durante los 16ms (a 60Hz) en que deben estar visibles.
     *
     * @param xy Coordenadas del pixel que se quiere cambiar
     * @param color Color que se quiere poner en el pixel indicado
     */
    void set_pixel(Pt xy, int color);
};

}  // namespace pro2

#endif