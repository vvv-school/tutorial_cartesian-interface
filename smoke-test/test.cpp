/* 
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <string>
#include <cmath>
#include <algorithm>

#include <robottestingframework/dll/Plugin.h>
#include <robottestingframework/TestAssert.h>

#include <yarp/robottestingframework/TestCase.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

using namespace std;
using namespace robottestingframework;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

/**********************************************************************/
class TestTutorialCartesianInterface : public yarp::robottestingframework::TestCase
{
    PolyDriver drvCartArm;

public:
    /******************************************************************/
    TestTutorialCartesianInterface() :
        yarp::robottestingframework::TestCase("TestTutorialCartesianInterface")
    {
    }
    
    /******************************************************************/
    virtual ~TestTutorialCartesianInterface()
    {
    }
    
    /******************************************************************/
    virtual bool setup(yarp::os::Property& property)
    {
        string robot=property.check("robot",Value("icubSim")).asString();
       
        Property option;
        option.put("device","cartesiancontrollerclient");
        option.put("remote","/"+robot+"/"+"cartesianController/left_arm");
        option.put("local","/"+getName()+"/cartesian");

        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Opening Clients");
        // let's give the controller some time to warm up
        bool ok=false;
        double t0=Time::now();
        while (Time::now()-t0<10.0)
        {
            // this might fail if controller
            // is not connected to solver yet
            if (drvCartArm.open(option))
            {                
                ok=true;
                break;
            }
            
            Time::delay(1.0);
        }
        
        if (!ok)
            ROBOTTESTINGFRAMEWORK_ASSERT_FAIL("Unable to open Clients!");

        return true;
    }
    
    /******************************************************************/
    virtual void tearDown()
    {
        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Closing Clients");
        if (!drvCartArm.close())
            ROBOTTESTINGFRAMEWORK_ASSERT_FAIL("Unable to close Clients!");
    }
    
    /******************************************************************/
    virtual void run()
    {   
        ICartesianControl *iarm;
        drvCartArm.view(iarm);
     
        Vector c(3),od(4);
        c[0]=-0.3; c[1]=-0.1; c[2]=0.1;

        double mean_v=0;
        double mean_x=0;
        double stdev_x=0;        
        int N=0;

        Time::delay(5.0);        
        
        ROBOTTESTINGFRAMEWORK_TEST_REPORT("Checking the trajectory of the end-effector");
        for (double t0=Time::now(); Time::now()-t0<10.0; N++)
        {
            Vector x,o,xdot,qdot;
            iarm->getPose(x,o);
            iarm->getTaskVelocities(xdot,qdot);

            double d=norm(x-c);
            
            mean_v+=norm(xdot);
            mean_x+=d;
            stdev_x+=d*d;            

            Time::delay(0.1);
        }

        mean_v/=N;
        mean_x/=N;
        stdev_x=sqrt(stdev_x/N-mean_x*mean_x);
        
        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("velocity mean = %g [m]",mean_v));
        ROBOTTESTINGFRAMEWORK_TEST_CHECK(mean_v>0.01,"Unsteadiness Test Passed!");
        
        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("mean distance from the center = %g [m]",mean_x));
        ROBOTTESTINGFRAMEWORK_TEST_REPORT(Asserter::format("stdev distance from the center = %g [m]",stdev_x));        
        ROBOTTESTINGFRAMEWORK_TEST_CHECK(stdev_x<0.015,"Roundness Test Passed!");
    }
};

ROBOTTESTINGFRAMEWORK_PREPARE_PLUGIN(TestTutorialCartesianInterface)
