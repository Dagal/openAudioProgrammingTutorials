
// This tutorial builds upon the JackClient tutorial, so it is a good
// idea to look over there if you're not familiar with a simple JACK client

// Contact: Harry van Haaren <harryhaaren@gmail.com>
// Compile: g++ ladspaHost.cpp -o ladspaHost `pkg-config --cflags --libs jack glibmm-2.4`


// Intro
//    In order to load LADSPA plugins we need to use a library. More
//    technically, we need to load a "shared object". We're going to use
//    GLibmm to do this, specifically the Module class.
//    
//    What the glibmm::module class provides is a way to load these shared 
//    objects in a platform independant way. It also gives us access to
//    the interal functions in the object, which is how we're going to
//    get access to the audio processing functions. 
//    
//    The LADSPA effect we're going to host is the AM pitchshifter from
//    Steve Harris SWH plugin suite.
//    Details of plugin: http://plugin.org.uk/ladspa-swh/docs/ladspa-swh.html#tth_sEc2.5

#include <iostream>
#include <jack/jack.h>

// include the LADSPA header file
#include <ladspa.h>

// then include Glibmm's "Module", so we can load the plugin
#include <glibmm/module.h>

// here we have the "handle" to the LADSPA effect, this is how we will
// create an actual effect object later on
LADSPA_Handle ladspaHandle;

// next we define some global variables, like glibLadspaModule and LADSPA_Handle
// note these are pointers, so we initialize them to 0
Glib::Module* glibModule = 0;
LADSPA_Descriptor* descriptor = 0;


jack_port_t* inputPort = 0;
jack_port_t* outputPort = 0;


int process(jack_nframes_t nframes, void* )
{
  float* inputBuffer = (float*)jack_port_get_buffer ( inputPort , nframes);
  float* outputBuffer= (float*)jack_port_get_buffer ( outputPort, nframes);
  
  for ( int i = 0; i < (int) nframes; i++)
  {
    outputBuffer[i] = inputBuffer[i];
  }
  
  return 0;
}

int main()
{
  std::cout << "LADSPA Host tutorial" << std::endl;
  
  // create jack client
  jack_client_t* client = jack_client_open ("LadspaHost",
                                            JackNullOption,
                                            0,
                                            0 );
  
  // get some info about JACK
  int samplerate = jack_get_sample_rate( client );
  
  
  // LADSPA stuff starts here!
  ladspaModule = new Glib::Module( pluginString );
  
  
  
  // LADSPA stuff finishes here!
  
  // set up JACK & start processing
  jack_set_process_callback  (client, process , 0);
  
  inputPort = jack_port_register ( client,
                                    "input",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsInput,
                                    0 );
  
  outputPort = jack_port_register ( client,
                                    "output",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput,
                                    0 );
  
  jack_activate(client);
  
  sleep(30);
  
  jack_deactivate(client);
  
  jack_client_close(client);
  
  return 0;
}
