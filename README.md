# opentoonz_kmeans_posterize
Opentoonz Plugin to posterize photo

input | output(k=2)
--- | ---
<img src="https://raw.githubusercontent.com/Hi-king/opentoonz_kmeans_posterize/master/sample/hokusai.jpg" width="100%"> | <img src="https://raw.githubusercontent.com/Hi-king/opentoonz_kmeans_posterize/master/sample/hokusai.0001.png" width="100%">

# Install pre-built plugin

Now I only prepare OSX(Darwin) binary.
You can use this plugin with following

0. put `hiking_KmeansPosterize_Darwin.plugin` into `OpenToonz_1.0_stuff/plugins` directory
0. restart OpenToonz

# Build plugin by yourself

0. `(cd opentoonz_plugin_utility; cmake .; make)`
  * compile `opentoonz_plugin_utility` library first

0. `cmake .`
0. `make`
0. put `hiking_KmeansPosterize_${YOUR_PLATFORM}` into `OpenToonz_1.0_stuff/plugins` directory
0. restart OpenToonz
