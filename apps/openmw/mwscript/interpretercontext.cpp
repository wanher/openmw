
#include "interpretercontext.hpp"

#include <cmath>
#include <stdexcept>
#include <iostream>

#include <components/interpreter/types.hpp>

#include "../mwworld/world.hpp"

#include "locals.hpp"
#include "globalscripts.hpp"

namespace MWScript
{
    InterpreterContext::InterpreterContext (MWWorld::Environment& environment,
        MWScript::Locals *locals, MWWorld::Ptr reference)
    : mEnvironment (environment), mLocals (locals), mReference (reference)
    {}

    int InterpreterContext::getLocalShort (int index) const
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");
     
        return mLocals->mShorts.at (index);
    }

    int InterpreterContext::getLocalLong (int index) const
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");
    
        return mLocals->mLongs.at (index);
    }

    float InterpreterContext::getLocalFloat (int index) const
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");
    
        return mLocals->mFloats.at (index);
    }

    void InterpreterContext::setLocalShort (int index, int value)
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");

        mLocals->mShorts.at (index) = value;
    }

    void InterpreterContext::setLocalLong (int index, int value)
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");

        mLocals->mLongs.at (index) = value;    
    }

    void InterpreterContext::setLocalFloat (int index, float value)
    {
        if (!mLocals)
            throw std::runtime_error ("local variables not available in this context");
    
        mLocals->mFloats.at (index) = value;    
    }
    
    void InterpreterContext::messageBox (const std::string& message,
        const std::vector<std::string>& buttons)
    {
        std::cout << "message box: " << message << std::endl;
        
        if (!buttons.empty())    
            std::cerr << "error: message box buttons not supported" << std::endl;
    }
    
    bool InterpreterContext::menuMode()
    {
        return false;
    }
    
    int InterpreterContext::getGlobalShort (const std::string& name) const
    {
        Interpreter::Type_Data value = mEnvironment.mWorld->getGlobalVariable (name);       
        return static_cast<Interpreter::Type_Short> (
            *reinterpret_cast<Interpreter::Type_Integer *> (&value));
    }

    int InterpreterContext::getGlobalLong (const std::string& name) const
    {
        // a global long is internally a float.
        Interpreter::Type_Data value = mEnvironment.mWorld->getGlobalVariable (name);       
        return static_cast<Interpreter::Type_Integer> (
            *reinterpret_cast<Interpreter::Type_Float *> (&value));
    }

    float InterpreterContext::getGlobalFloat (const std::string& name) const
    {
        Interpreter::Type_Data value = mEnvironment.mWorld->getGlobalVariable (name);       
        return *reinterpret_cast<Interpreter::Type_Float *> (&value);
    }

    void InterpreterContext::setGlobalShort (const std::string& name, int value)
    {
         mEnvironment.mWorld->getGlobalVariable (name) =
            *reinterpret_cast<Interpreter::Type_Data *> (&value);
    }

    void InterpreterContext::setGlobalLong (const std::string& name, int value)
    {
        // a global long is internally a float.
        float value2 = float(value);

         mEnvironment.mWorld->getGlobalVariable (name) =
            *reinterpret_cast<Interpreter::Type_Data *> (&value2);    
    }

    void InterpreterContext::setGlobalFloat (const std::string& name, float value)
    {
         mEnvironment.mWorld->getGlobalVariable (name) =
            *reinterpret_cast<Interpreter::Type_Data *> (&value);    
    }
     
    bool InterpreterContext::isScriptRunning (const std::string& name)
    {
        return mEnvironment.mGlobalScripts->isRunning (name);
    }
    
    void InterpreterContext::startScript (const std::string& name)
    {
        mEnvironment.mGlobalScripts->addScript (name);
    }
    
    void InterpreterContext::stopScript (const std::string& name)
    {
        mEnvironment.mGlobalScripts->removeScript (name);
    }
    
    float InterpreterContext::getDistance (const std::string& name)
    {
        if (mReference.isEmpty())
            throw std::runtime_error ("no implicit reference");
            
        std::pair<MWWorld::Ptr, MWWorld::World::CellStore *> ref =
            mEnvironment.mWorld->getPtr (name, true);
                       
        double diff[3];
        
        for (int i=0; i<3; ++i)                            
            diff[i] = ref.first.getCellRef().pos.pos[i] - mReference.getCellRef().pos.pos[i];
        
        return std::sqrt (diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
    }
    
    bool InterpreterContext::hasBeenActivated() const
    {
        return false;
    }
    
    float InterpreterContext::getSecondsPassed() const
    {
        return mEnvironment.mFrameDuration;
    }
    
    MWWorld::World& InterpreterContext::getWorld()
    {
        return *mEnvironment.mWorld;
    }
    
    MWSound::SoundManager& InterpreterContext::getSoundManager()
    {
        return *mEnvironment.mSoundManager;
    }
    
    MWWorld::Ptr InterpreterContext::getReference()
    {
        return mReference;
    }
}
