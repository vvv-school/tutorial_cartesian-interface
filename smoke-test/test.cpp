/* 
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini <ugo.pattacini@iit.it>
 * CopyPolicy: Released under the terms of the GNU GPL v3.0.
*/

#include <string>
#include <algorithm>

#include <rtf/yarp/YarpTestCase.h>
#include <rtf/dll/Plugin.h>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

using namespace std;
using namespace RTF;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

/**********************************************************************/
class TestTutorialCartesianInterface : public YarpTestCase
{
    yarp::dev::PolyDriver drvCartArm;

public:
    /******************************************************************/
    TestTutorialCartesianInterface() :
        YarpTestCase("testTutorialCartesianInterface")
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
        option.put("remote",("/"+robot+"/"+"cartesianController/left_arm"));
        option.put("local",("/"+getName()+"/cartesian"));

        RTF_TEST_REPORT("Opening Clients");        
        RTF_ASSERT_ERROR_IF(drvCartArm.open(option),"Unable to open Clients!");

        return true;
    }
    
    /******************************************************************/
    virtual void tearDown()
    {
        RTF_TEST_REPORT("Closing Clients");
        RTF_ASSERT_ERROR_IF(drvCartArm.close(),"Unable to close Clients!");
    }
    
    /******************************************************************/
    virtual void run()
    {        
        Vector c(3),od(4);
        c[0]=-0.3; c[1]=-0.1; c[2]=0.1;
        double R=0.1;
        
        ICartesianControl *iarm;
        drvCartArm.view(iarm);
        
        Vector x0;
        Time::delay(5.0);
        
        RTF_TEST_REPORT("Checking the trajectory of the end-effector");
        double t0=Time::now();
        for (int i=0; Time::now()-t0<5.0; i++)
        {
            Vector x1,o1;
            iarm->getPose(x1,o1);
                        
            double d=norm(x1-c);
            
            if (i>0)
                RTF_ASSERT_ERROR_IF(norm(x1-x0)<0.01,"The arm seems stationary!");
            RTF_ASSERT_ERROR_IF(d>1.2*R,Asserter::format("Arm too far from the center! d=%g [m]",d));
            RTF_ASSERT_ERROR_IF(d<0.8*R,Asserter::format("Arm too close to the center! d=%g [m]",d));
            
            x0=x1;
            
            Time::delay(0.5);
        }
        
        RTF_TEST_CHECK(true,"Test Passed!");
    }
};

PREPARE_PLUGIN(TestTutorialCartesianInterface)
