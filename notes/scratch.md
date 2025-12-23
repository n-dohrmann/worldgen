# Scratch Notes

**a loose collection of thoughts**

I'm sure there's plenty of detailed literature on realistic (or at least not
outright ridiculous) world generation algos, but I think it might be fun to do
the first implementation blind. After finding out what's working and what's not
working, the approach can be improved. this project is really just for my own
fun so I'm not worried about hyperrealism.

---

## Worldgen Desiderata

Basic field information:

- elevation
- avg summer and winter temperature
- avg humidity (or precipitation)
- avg wind current vector
- avg sun exposure (wattage)

## Generation Stages

Thinking we can have a pipeline of sorts that progressively adds detail to the
world.

### 1: Elevation

#### Design

e.g. the first stage would probably have to be generating elevation - all such
points below a threshold would be considered water. Points above a certain
threshold should be considered high elevation and experience certain effects
(this would likely be handled at a later stage in the pipeline however, such as
having more extreme temperatures). Areas of the map with a high gradient in
elevation should be flagged as boundaries of mountain ranges.

#### Algorithms

### 2: Temperature and Windcurrents

#### Design

These two properties may be interdependent. At a base level, temperature could
be estimated by distance to the equator, with the coldest regions clearly being
those farthest away. At the same time, it should also be possible to simulate
warm/cold air currents influencing temperatures from one region to the next. The
inclusion of wind currents also begs the question of how the rectangular map
boundaries will be handled. Is the goal to simulate a rectangular projection of
a spherical surface? Or is a non-physical representation with simple periodic
boundaries ok? (e.g. east connected to west, but north and south with hard
boundaries). As long as the wind vector field is not too "jagged" and we
minimize divergence of the field, the result should be good enough.

#### Algorithms

### 3: Humidity and Precipitation

#### Design

#### Algorithms

### 4. Biome Designation

#### Design

#### Algorithms

### 5. Rendering
