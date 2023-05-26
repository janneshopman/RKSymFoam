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
(NRG), Westerduinweg 3, 1755 LE Petten, The Netherlands. The
symmetry-preserving method was applied to this structure by Jannes Hopman,
of the Heat and Mass Transfer Technological Center, Technical University
of Catalonia, C/Colom 11, 08222 Terrassa, Spain. 

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

<pre>
./Allwmake
</pre>

## Test cases

* All test cases can be found in the "cases" directory, including a
Taylor-Green Vortex and a channel flow.
* Both test cases contain a "templateCase" directory, which should be
copied before it run from inside the new directory with:

<pre>
./run.sh &ltsolver&gt &ltsimulation type&gt &ltRunge-Kutta scheme&gt
</pre> 

* Solvers permitted by this script: "icoFoam", "pimpleFoam" and
"RKSymFoam"
* Simulation types permitted by this script: "LES", "laminar" (laminar
should be selected to run DNS)
* Runge-Kutta schemes permitted by this script: "BackwardEuler", "Kutta"
(classical Runge-Kutta 3 scheme)
* icoFoam does not read the \<simulation type\> and \<Runge-Kutta scheme\>
arguments, so they can be omitted
* pimpleFoam does not read the \<Runge-Kutta scheme\> argument, so it can
be omitted
* The user is encouraged to experiment with different settings after
getting familiar with the structure of the code, to do so change the
\<VAR*\> variables inside "system/controldict.m4",
"constant/turbulenceProperties.m4" and "system/fvSolution.m4" and rename
the files to omit the ".m4" extension.
* The available Runge-Kutta schemes can be found in
"libraries/RungeKuttaSchemes/", the Butcher Tableaus are given in the
<.C> file and a reference is given in the <.H> file

### Taylor-Green Vortex

* Demonstrating the loss of kinetic energy due to numerical dissipation
over time
* Validation cases: icoFoam and RKSymFoam using Backward Euler scheme 

### Channel flow

* Demonstrating accuracy of the solver to simulate turbulence
* Demonstrating the ability to include LES models 
* Validation cases:
* DNS cases: icoFoam (Backward Euler) and RKSymFoam (Backward Euler and
Runge-Kutta 3)
* LES cases: pimpleFoam (Backward Euler) and RKSymFoam (Backward Euler and
Runge-Kutta 3)
* Run cases on 8 processors with the same command by first adjusting the
following lines in run.sh to:

<pre> 
#- Run serial
# runApplication $(getApplication)

#- Run parallel 
runApplication decomposePar 
runParallel $(getApplication)
runApplication reconstructPar 
</pre>

### Post-processing

* To post-process the cases, run "plot.py" from the 
"cases/\<case\>/postProcess" directory using

<pre>
python plot.py
</pre>

* For example: To postprocess \<run_directory\>/\<case_1_name\> and 
\<run_directory\>/\<case_2_name\>, edit the \<runDir\> and \<solvers\> 
variables in "plot.py" to:

<pre>
runDir = &ltrun_directory&gt

solvers = ['&ltcase_1_name&gt', '&ltcase_2_name&gt']
</pre>

* Resulting plots will be found in the "postProcess/results" directory

### Validation 

* A set of predetermined cases was run and post-processed by the authors.
* To run the same cases and reproduce the results, navigate to the
"cases/validation" directory and run all cases with:

<pre>
./launchCases.sh
</pre>

* After completing the cases, run "plot.py" inside 
"cases/\<case\>postProcessing", there is no need to alter "plot.py"
* The resulting plots can be compared with the results readily available
in "cases/\<case\>/postProcessing/validationResults"

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
} 
</pre>

* All available schemes are based on the Butcher Tableau and can be found
in the "libraries/RungeKuttaSchemes" directory
* Cases are run exactly the same way as by any other OpenFOAM solver
* A transport model has to be chosen in "constant/tansportProperties",
similar to the usage of pimplFoam.
* A turbulence model has to be chosen in "constant/turbulenceProperties"
file, similar to the usage of pimpleFoam.
* If you want to run a DNS, set the transport model to Newtonian (1) and
the simulation type to laminar (2), as demonstrated below.
* 1\. In "constant/transportProperties" add the line:

<pre>
transportModel  Newtonian;
</pre>

* 2\. Create the file constant/turbulenceProperties:
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
