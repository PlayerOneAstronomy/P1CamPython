# Player One Camera SDK python wrapper
This project is based on [cmake_example for pybind11](https://github.com/pybind/cmake_example), means that the Player One camera SDK's python bindings is implemented by using [pybind11](https://github.com/pybind/pybind11).


## Prerequisites

* A compiler with C++11 support
* Pip 10+ or CMake >= 3.4 (or 3.14+ on Windows, which was the first version to support VS 2019)
* Ninja or Pip 10+


## Installation

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/PlayerOneAstronomy/P1CamPython
pip install ./P1CamPython
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.
Note: network proxy may cause some errors when building.



## Building the documentation

Documentation for the example project is generated using Sphinx. Sphinx has the
ability to automatically inspect the signatures and documentation strings in
the extension module to generate beautiful documentation in a variety formats.
The following command generates HTML-based reference documentation; for other
formats please refer to the [Sphinx](https://www.sphinx-doc.org/en/master/usage/installation.html#installation-from-pypi) manual:

 - `cd P1CamPython/docs`
 - `make html`


## License

This project is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.


## Test call

```python
import player_one_camera as P1Cam
list_cam_prop = P1Cam.ListCamProp()
P1Cam.GetAllCameraProperties(list_cam_prop)
print('camera count: {}'.format(len(list_cam_prop)))
```


## Example

In the `examples` folder, there is an example file `example_basic.py`, which shows the usage of each API, you can also run it directly to view the running results:
```bash
python3 example_basic.py # on windows, python example_basic.py
```