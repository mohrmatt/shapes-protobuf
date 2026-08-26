/*
* (c) Copyright, Real-Time Innovations, 2020.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#include <iostream>
#include <random>

#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp>      // for sleep()
#include <rti/config/Logger.hpp>  // for logging
// alternatively, to include all the standard APIs:
//  <dds/dds.hpp>
// or to include both the standard APIs and extensions:
//  <rti/rti.hpp>
//
// For more information about the headers and namespaces, see:
//    https://community.rti.com/static/documentation/connext-dds/7.7.0/doc/api/connext_dds/api_cpp2/group__DDSNamespaceModule.html
// For information on how to use extensions, see:
//    https://community.rti.com/static/documentation/connext-dds/7.7.0/doc/api/connext_dds/api_cpp2/group__DDSCpp2Conventions.html

#include "application.hpp"  // for command line parsing and ctrl-c
#include "ShapeType_Key.hpp"

void run_publisher_application(unsigned int domain_id, unsigned int sample_count)
{
    // DDS objects behave like shared pointers or value types
    // (see https://community.rti.com/best-practices/use-modern-c-types-correctly)

    // Start communicating in a domain, usually one participant per application
    dds::domain::DomainParticipant participant(domain_id);

    // Create a Topic with a name and a datatype
    dds::topic::Topic< ::ShapeType> topic(participant, "Circle");

    // Create a Publisher
    dds::pub::Publisher publisher(participant);

    // Create a DataWriter with default QoS
    dds::pub::DataWriter< ::ShapeType> writer(publisher, topic);

    // Create two instances: blue and green circles
    ::ShapeType blue_data;
    blue_data.set_color("blue");
    blue_data.set_shapesize(50);
    
    ::ShapeType green_data;
    green_data.set_color("green");
    green_data.set_shapesize(40);

    // Register each keyed instance before writing
    dds::core::InstanceHandle blue_handle = writer.register_instance(blue_data);
    dds::core::InstanceHandle green_handle = writer.register_instance(green_data);
    
    // Initialize random number generator for x and y coordinates
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 250);
    
    // Track green samples sent
    unsigned int green_samples_sent = 0;
    
    // Main loop, write data
    for (unsigned int samples_written = 0;
    !application::shutdown_requested && samples_written < sample_count;
    samples_written++) {
        // Write blue circle instance using protobuf setter methods
        blue_data.set_x(dis(gen));
        blue_data.set_y(dis(gen));
        
        std::cout << "Writing ::ShapeType, count " << samples_written 
                  << " (color: " << blue_data.color() 
                  << ", x: " << blue_data.x() 
                  << ", y: " << blue_data.y() 
                  << ", shapesize: " << blue_data.shapesize() << ")" << std::endl;

        writer.write(blue_data, blue_handle);
        
        // Write green circle instance only for first 2 iterations
        if (green_samples_sent < 2) {
            green_data.set_x(dis(gen));
            green_data.set_y(dis(gen));
            
            std::cout << "Writing ::ShapeType, count " << samples_written 
                      << " (color: " << green_data.color() 
                      << ", x: " << green_data.x() 
                      << ", y: " << green_data.y() 
                      << ", shapesize: " << green_data.shapesize() << ")" << std::endl;

            writer.write(green_data, green_handle);
            green_samples_sent++;
        }

        // Send once every second
        rti::util::sleep(dds::core::Duration(1));
    }

    // Unregister instances to notify subscribers these instances are gone
    writer.unregister_instance(blue_handle);
    writer.unregister_instance(green_handle);
}

int main(int argc, char *argv[])
{

    using namespace application;

    // Parse arguments and handle control-C
    auto arguments = parse_arguments(argc, argv);
    if (arguments.parse_result == ParseReturn::exit) {
        return EXIT_SUCCESS;
    } else if (arguments.parse_result == ParseReturn::failure) {
        return EXIT_FAILURE;
    }
    setup_signal_handlers();

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    try {
        run_publisher_application(arguments.domain_id, arguments.sample_count);
    } catch (const std::exception& ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in run_publisher_application(): " << ex.what()
        << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application exit
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}
