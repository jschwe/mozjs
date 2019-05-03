// Copyright (c) 2012 Ecma International.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
esid: sec-array.prototype.foreach
es5id: 15.4.4.18-5-25
description: Array.prototype.forEach - thisArg not passed
flags: [noStrict]
---*/

        function innerObj() {
            this._15_4_4_18_5_25 = true;
            var _15_4_4_18_5_25 = false;
            var result;
            function callbackfn(val, idx, obj) {
                result = this._15_4_4_18_5_25;
            }
            var arr = [1];
            arr.forEach(callbackfn)
            this.retVal = !result;
        }

assert(new innerObj().retVal, 'new innerObj().retVal !== true');

reportCompare(0, 0);