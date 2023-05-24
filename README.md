# RKSymFoam

This code contains slightly adjusted versions of the solvers used in the
paper "A symmetry-preserving second-order time-accurate PISO-based
method." by E.M.J. Komen, J.A. Hopman, E.M.A. Frederix, F.X. Trias and
R.W.C.P.  Verstappen. One notable adjustment is made in the pressure
gradient interpolation to follow the paper "An energy-preserving 
unconditionally stable fractional step method on collocated grids." by D.
Santos Serrano, F.X. Trias Miquel, G. Colomer Rey and C.D. Pérez Segarra.
For a description of the method, please refer to these papers.

## Authors

The main structure of the solver, including the Runge-Kutta schemes was
developped by Edo Frederix, of the Nuclear Research and Consultancy Group 
(NRG), Westerduinweg 3, 1755 LE Petten, The Netherlands. The symmetry-
preserving method was applied to this structure by Jannes Hopman, of the 
Heat and Mass Transfer Technological Center, Technical University of 
Catalonia, C/Colom 11, 08222 Terrassa, Spain. 

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
* Run cases from the TGV directory using

<pre> ./launchCases.sh </pre>

### Channel flow

* Demonstrating accuracy of the solver to simulate turbulence
* Demonstrating the ability to include LES models 
* DNS cases: icoFoam (Backward Euler) and RKSymFoam (Backward Euler and 
Runge-Kutta 3)
* LES cases: pimpleFoam (Backward Euler) and RKSymFoam (Backward Euler and
Runge-Kutta 3)
* Run cases on a single processor from their directories using

<pre> ./launchCases.sh </pre>

* Run cases on 8 processors with the same command by first adjusting
the following lines in templateCase/run.sh to:

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

* The entries in system/fvSchemes are not read by RKSymFoam, except 
potentially for the turbulence model, all other schemes can be set to:

<pre> 
    default         none;
</pre>

* In system/fvSolution, the subdictionaries for p and pFinal are named
pCorr and pCorrFinal respectively.

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
LES             false;
} 
</pre>

* All available schemes are based on the Butcher Tableau and can be found
in the libraries/RungeKuttaSchemes directory
* Cases are run exactly the same way as by any other OpenFOAM solver
* A transport model has to be chosen in the constant/tansportProperties 
file, similar to the usage of pimplFoam. This will be used if the LES 
variable above is set to true.
* A turbulence model has to be chosen in the constant/turbulenceProperties
file, similar to the usage of pimpleFoam. This will be used if the LES
variable above is set to true.
* If running DNS, set LES to false, as shown above. In this case the
chosen model will not be used, but need to be present as dummy variables.
* Example transport model, in constant/transportProperties add the line:

<pre>
transportModel  Newtonian;
</pre>

* Example turbulence model, create the file constant/turbulenceProperties:
<pre>
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "constant";
    object      turbulenceProperties;
}

simulationType laminar;
</pre>

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
