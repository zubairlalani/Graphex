#include <visualizer/vector_field_simulator.h>

using vectorfield::visualizer::FieldSimulator;

void prepareSettings(FieldSimulator::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(FieldSimulator, ci::app::RendererGl, prepareSettings);
