# Fractal Visualizer
An ImGui and OpenGL based visualizer which takes in a simple expression and generates fractal patterns by evaluating that expression in the complex plane.

![](/images/mandelbrot.png)
![](/images/burning.png)

## Installation
Clone the repository :
```bash
placeholder
```
Inside the folder, go to FractalVisualizer and then double-click/run the FractalVisualizer.exe file

> Note: If its not working, make sure your computer has [Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist) installed.

## Controls
```Left-click + Drag``` - Pan around the fractal<br/>
```Sroll Wheel``` - Zoom in and out of the fractal

## Features
### Operations and Functions
You can add these operations and functions for the equation.

| **Operators**           | **Variables** | **Arithmetic / Complex Functions**                              |
|-------------------------|---------------|-----------------------------------------------------------------|
| `+`, `-`                | `z`           | `abs`, `sqrt`, `exp`, `log`, `ln`, `mod`, `conj`, `real`, `imag`|
| `*`, `/`                | `c`           | `sin`, `cos`, `tan`, `asin`, `acos`, `atan`                     |
| `^`                     |               | `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`               |

> Note: The equation parser does not work if you use implicit multiplication. For example, use "3*x" instead of "3x".<br/>
> Another Note: "real" and "imag" functions returns the real and imaginary values of the complex number respectively. It discards the other value when used.

### Properties
- Customize the fractal's color gradient and its contrast.
- Adjust the number of iterations for the fractal.
- Change the escape radius threshold for the fractal.

![](/images/visual.png)
- Supports custom made variables created by the user.
- Adjust the real and imaginary values present in the variable.

![](/images/variables.png)

## Example Equations
Keep in mind that all equations must include the variable 'z'.

- ```z^n + c```
- ```1 / (z^2 + c)```
- ```(sin(z) + cos(z))^2 + c```
- ```tan(z)^2 + c```
- ```z - (z^3 - var2) / (3*z^5) / (c^2) + var1```
- ```conj(exp(sin(z)^2 + c)) / (acos(z^2)) + var1```
- ```sinh(z)/cosh(z)*tanh(z) + var1```
- ```sin(3*z^3) * cos(3*real(z^2)) / tan(real(z)) + imag(c)```

## Pictures

![](/images/julia.png)
![](/images/newton.png)
![](/images/tricorn.png)

## Libraries Used
- [OpenGL](https://www.opengl.org/) - Core Graphics API
- [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics library
- [glad](https://glad.dav1d.de/) - OpenGL Function Loader
- [Dear ImGui](https://github.com/ocornut/imgui) - Immediate Mode GUI for the interactive interface and controls


Any feedback is welcome!
