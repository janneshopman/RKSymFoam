# RKSymFoam

This code contains slightly adjusted versions of the solvers used in the
paper "A symmetry-preserving second-order time-accurate PISO-based
method." by E.M.J. Komen, J.A. Hopman, E.M.A. Frederix, F.X. Trias and
R.W.C.P.  Verstappen. One notable adjustment is made in the pressure
gradient interpolation to follow the paper "On the interpolation problem
for the Poisson equation on collocated meshes." by D. Santos Serrano, D.
Muela N.  Valle Marchante and F.X. Trias Miquel. For a description of the
method, please refer to these papers.

## Authors

The main structure of the solver, including the Runge-Kutta schemes was
developped by Edo Frederix. The symmetry-preserving method was applied to
this structure by Jannes Hopman. 

## License

RKSymFoam is published under the GNU GPL Version 3 license, which can be
found in the LICENSE file.

## Prerequisites

* OpenFOAM v2012. While it may compile against other versions, this is not
tested and currently not supported.
* Python with numpy and matplotlib

## Usage

* Make sure that OpenFOAM v2012 is loaded into your environment 
* Compile all libraries and apps with

<pre> ./Allwmake </pre>

## Test cases

* All test cases can be found in the cases directory, including a
Taylor-Green Vortex and a channel flow

### Taylor-Green Vortex

* Demonstrating the loss of kinetic energy due to numerical dissipation
over time
* Comparison between icoFoam and RKSymFoam using Backward Euler scheme 
* Run cases from their directories using

<pre> ./run.sh </pre>

### Channel flow

* Demonstrating accuracy of the solver to simulate turbulence
* Demonstrating the ability to include LES models DNS cases: icoFoam
(Backward Euler) and RKSymFoam (Backward Euler and Runge-Kutta 3)
* LES cases: pimpleFoam (Backward Euler) and RKSymLESFoam (Backward Euler
and Runge-Kutta 3)
* Run cases on a single processor from their directories using

<pre> ./run.sh </pre>

* Run cases on 8 processors from their directories by first adjusting
run.sh to:

<pre> 
#- Run serial
# runApplication $(getApplication)

#- Run parallel 
runApplication decomposePar 
runParallel $(getApplication)
runApplication reconstructPar 
</pre>

### Post-processing

* To post-process the cases, run plot.py from the postprocessing directory
using

<pre> python plot.py </pre>

## Using RKSymFoam in your own OpenFOAM cases

* The entries in system/fvSchemes are not read by RKSymFoam, therefore all
schemes can be set to:

<pre> 
    default         none;
</pre>

* A subdictionary named RungeKutta has to be added to system/fvSolution,
for example:

<pre> 
RungeKutta 
{ 
scheme          BackwardEuler; 
nOuter          1;
nInner          2; 
pnPredCoef      1; 
pRefCell        0; 
pRefValue       0; 
} 
</pre>

* All available schemes are based on the Butcher Tableau and can be found
in the libraries/RungeKuttaSchemes directory
* Cases are run exactly the same as by any other OpenFOAM solver

### Using RKSymLESFoam

* RKSymLESFoam can request schemes from system/fvSchemes, depending on the
* choses LES model (grad(U) under gradSchemes and wallDist were defined 
for the test case)
* A transport model has to be chosen in the constant/transportProperties
file, similar to the usage of pimpleFoam
* A turbulence model has to be chosen in the
constant/turbulenceProperties file, similar to the usage of pimpleFoam

## Contact & support

For bug reports or support, feel free to contact Jannes Hopman at
jannes.hopman@upc.edu. Please note that this code is not maintained nor
regularly updated, and is only tested with OpenFOAM v2012. Questions
related to other versions will thus not be answered.

## Disclaimer

RKSymFoam is provided by the copyright holders and contributors "as-is"
and any express or implied warranties, including, but not limited to, the
implied warranties of merchantability and fitness for a particular purpose
are disclaimed. In no event shall the copyright owner or contributors be
liable for any direct, indirect, incidental, special, exemplary, or
consequential damages (including, but not limited to, procurement of
substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in
contract, strict liability, or tort (including negligence or otherwise)
arising in any way out of the use of this software, even if advised of the
possibility of such damage.
