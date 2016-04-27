/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef vm_Debugger_inl_h
#define vm_Debugger_inl_h

#include "vm/Debugger.h"

#include "vm/Stack-inl.h"

/* static */ inline bool
js::Debugger::onLeaveFrame(JSContext* cx, AbstractFramePtr frame, jsbytecode* pc, bool ok)
{
    MOZ_ASSERT_IF(frame.isInterpreterFrame(), frame.asInterpreterFrame() == cx->interpreterFrame());
    MOZ_ASSERT_IF(frame.script()->isDebuggee(), frame.isDebuggee());
    /* Traps must be cleared from eval frames, see slowPathOnLeaveFrame. */
    mozilla::DebugOnly<bool> evalTraps = frame.isEvalFrame() &&
                                         frame.script()->hasAnyBreakpointsOrStepMode();
    MOZ_ASSERT_IF(evalTraps, frame.isDebuggee());
    if (frame.isDebuggee())
        ok = slowPathOnLeaveFrame(cx, frame, pc, ok);
    MOZ_ASSERT(!inFrameMaps(frame));
    return ok;
}

/* static */ inline js::Debugger*
js::Debugger::fromJSObject(const JSObject* obj)
{
    MOZ_ASSERT(js::GetObjectClass(obj) == &jsclass);
    return (Debugger*) obj->as<NativeObject>().getPrivate();
}

/* static */ inline bool
js::Debugger::checkNoExecute(JSContext* cx, HandleScript script)
{
    if (!cx->compartment()->isDebuggee() || !cx->runtime()->noExecuteDebuggerTop)
        return true;
    return slowPathCheckNoExecute(cx, script);
}

/* static */ JSTrapStatus
js::Debugger::onEnterFrame(JSContext* cx, AbstractFramePtr frame)
{
    MOZ_ASSERT_IF(frame.script()->isDebuggee(), frame.isDebuggee());
    if (!frame.isDebuggee())
        return JSTRAP_CONTINUE;
    return slowPathOnEnterFrame(cx, frame);
}

/* static */ JSTrapStatus
js::Debugger::onDebuggerStatement(JSContext* cx, AbstractFramePtr frame)
{
    if (!cx->compartment()->isDebuggee())
        return JSTRAP_CONTINUE;
    return slowPathOnDebuggerStatement(cx, frame);
}

/* static */ JSTrapStatus
js::Debugger::onExceptionUnwind(JSContext* cx, AbstractFramePtr frame)
{
    if (!cx->compartment()->isDebuggee())
        return JSTRAP_CONTINUE;
    return slowPathOnExceptionUnwind(cx, frame);
}

/* static */ void
js::Debugger::onNewWasmModule(JSContext* cx, Handle<WasmModuleObject*> wasmModule)
{
    // Insert the wasm::Module into a compartment-wide list for discovery
    // later without a heap walk.
    cx->compartment()->wasmModuleWeakList.insertBack(&wasmModule->module());
    if (cx->compartment()->isDebuggee())
        slowPathOnNewWasmModule(cx, wasmModule);
}

#endif /* vm_Debugger_inl_h */
