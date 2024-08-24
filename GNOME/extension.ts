/* extension.js
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import Gio from 'gi://Gio';
import {Extension} from 'resource:///org/gnome/shell/extensions/extension.js';

const MR_DBUS_IFACE = `
<node>
    <interface name="org.gnome.Shell.Extensions.ArkPets">
        <method name="MoveResize">
            <arg type="u" direction="in" name="winid" />
            <arg type="i" direction="in" name="x" />
            <arg type="i" direction="in" name="y" />
            <arg type="u" direction="in" name="width" />
            <arg type="u" direction="in" name="height" />
        </method>
        <method name="Activate">
            <arg type="u" direction="in" name="winid" />
        </method>
        <method name="Above">
            <arg type="u" direction="in" name="winid" />
        </method>
        <method name="Unabove">
            <arg type="u" direction="in" name="winid" />
        </method>
        <method name="List">
            <arg type="au" direction="out" name="win" />
        </method>
        <method name="Visible">
            <arg type="u" direction="in" name="winid" />
            <arg type="b" direction="out" name="visible" />
        </method>
        <method name="Text">
            <arg type="u" direction="in" name="winid" />
            <arg type="(ss)" direction="out" name="titleclass" />
        </method>
        <method name="Rect">
            <arg type="u" direction="in" name="winid" />
            <arg type="(iiuu)" direction="out" name="xywh" />
        </method>
        <method name="IsActive">
            <arg type="u" direction="in" name="winid" />
            <arg type="b" direction="out" name="active" />
        </method>
        <method name="Alpha">
            <arg type="u" direction="in" name="winid" />
            <arg type="u" direction="in" name="alpha" />
        </method>
        <method name="Version">
            <arg type="u" direction="out" name="ver"/>
        </method>
    </interface>
</node>`;

export default class ArkPetsIntegrationExtension extends Extension {
    _dbus?: Gio.DBusExportedObject;
    override enable() {
        this._dbus = Gio.DBusExportedObject.wrapJSObject(MR_DBUS_IFACE, this);
        this._dbus.export(
            Gio.DBus.session,
            '/org/gnome/Shell/Extensions/ArkPets'
        );
    }

    override disable() {
        if (this._dbus) {
            this._dbus.flush();
            this._dbus.unexport();
        }
        delete this._dbus;
    }

    _get_window_by_wid(winid: number) {
        const win = global
            .get_window_actors()
            .find(w => w.meta_window.get_id() === winid);
        return win;
    }

    Version() { return 1; }

    Visible(winid: number) {
        const win = this._get_window_by_wid(winid);
        const workspaceManager = global.workspace_manager;
        if (win) {
            const in_current_workspace = win.meta_window.located_on_workspace(
                workspaceManager.get_active_workspace()
            );
            const minimized = win.meta_window.minimized;
            if (!in_current_workspace || minimized) {
                return false;
            }
            return true;
        } else {
            console.debug('Not found');
            return false;
        }
    }

    List() {
        const win = global.get_window_actors();
        const winIdArr = win.map(w => w.meta_window.get_id());
        return winIdArr;
    }

    Rect(winid: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            const rect = win.meta_window.get_frame_rect();
            return [rect.x, rect.y, rect.width, rect.height];
        } else {
            console.debug('Not found');
            return [0, 0, 0, 0];
        }
    }

    Text(winid: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            let title = win.meta_window.get_title();
            let wmclass = win.meta_window.get_wm_class();
            if (!title) {
                title = '';
            }
            if (!wmclass) {
                wmclass = '';
            }
            return [title, wmclass];
        } else {
            console.debug('Not found');
            return ['', ''];
        }
    }

    MoveResize(
        winid: number,
        x: number,
        y: number,
        width: number,
        height: number
    ) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            if (
                win.meta_window.maximized_horizontally ||
                win.meta_window.maximized_vertically
            ) {
                win.meta_window.unmaximize(3);
            }
            win.meta_window.move_resize_frame(true, x, y, width, height);
        } else {
            console.debug('Not found');
        }
    }

    Activate(winid: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            win.meta_window.activate(0);
        } else {
            console.debug('Not found');
        }
    }

    Above(winid: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            win.meta_window.make_above();
        } else {
            console.debug('Not found');
        }
    }

    Unabove(winid: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            win.meta_window.unmake_above();
        } else {
            console.debug('Not found');
        }
    }

    IsActive(winid: number) {
        return global.get_display().get_focus_window().get_id() === winid;
    }

    Alpha(winid: number, alpha: number) {
        const win = this._get_window_by_wid(winid);
        if (win) {
            win.set_opacity(alpha);
        } else {
            console.debug('Not found');
        }
    }
}
