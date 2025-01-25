## Purpose ##
Calculate the position of a planet within our solar system at a given point in time. Display it's distance away from earth.

## Definitions ##
Orbital Elements
: the information needed to define a position of a planet in orbit. These parameters specify the angle of orbit in three dimensions and the position of the planet along that orbit. Measurements are relative to the J2000 epoch (Jan 1, 2000) and/or to the plane of the ecliptic (the imaginary plane containing the Earth's orbit around the Sun) where specified.

Semi-major Axis
: [units: AU] half of the length of the long axis of the ellipse (where AU is an astronomical unit, the mean distance between the Earth and the Sun)

Eccentricity
: [units: N/A] ratio between the focal distance (which is relative to J2000) and the semi-major axis

Orbital Inclination
: [units: degrees] The tilt of the planet's orbit around the sun relative to plane of the ecliptic

Longitude of the Ascending Node
: [units: degrees] angular position on plane of the ecliptic relative to J2000

Longitude of Perihelion
: [units: degrees] the point in orbit closest to sun relative to the plane of the ecliptic. All three anomalies (calculated later on) are zero when the planet is in perihelion. The longitude of the ascending node subtracted from this angle is known as the argument of the periapsis

Mean Anomaly
: [units: degrees] the fraction of the period that has elapsed since the planet passed through the perihelion relative to J2000

Period
: [units: days] refers to the time around the sun

## Visualization in 2D ##
![Visualization in 2D](https://cdn.britannica.com/25/5725-050-E66883FF/Anomaly-A-aphelion-B-perihelion-anomaly-orbit-C-centre-E-eccentric.jpg)

V = trueAnomaly, E = eccentricAnomaly, B = perihelion, P = planet, and the distance between A and B represents the major axis. The orbital inclination exists in the dimension not shown. NOTE: These variable letters reference the image and do not align with the rest of the program.

## Visualization in 3D ##
![Visualization in 3D](https://upload.wikimedia.org/wikipedia/commons/thumb/e/eb/Orbit1.svg/640px-Orbit1.svg.png)

The plane of reference is the orbital plane of Earth around the sun. The reference direction is referred to as "the first point of Aries". Like an epoch, it is an arbitrary reference point. It was defined over 2000 years ago by Hipparchus, considered the founder of trigonometry.

## Caveats ##
There are two main categories of planetary movement NOT accounted for which reduces our accuracy significantly:
1. nutation and precession: changes in the planet's axis of rotation
2. perturbations: forces other than the sun acting on the planet (other satellites). This especially effects the Jovian planets: Saturn, Jupiter, Uranus, and Neptune.

In addition, we simplify the problem so that all planets "freeze" during travel time. Accounting for the movement of planets during travel time is currently beyond the scope of this project.
