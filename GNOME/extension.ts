/** Copyright (c) 2024, Litwak913
 * At GPL-3.0 License
 */

import Gio from 'gi://Gio';
import {Extension} from 'resource:///org/gnome/shell/extensions/extension.js';
import Meta from "gi://Meta";

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
            <arg type="a(iiuussbu)" direction="out" name="win" />
        </method>
        <method name="IsActive">
            <arg type="u" direction="in" name="winid" />
            <arg type="b" direction="out" name="active" />
        </method>
        <method name="Version">
            <arg type="s" direction="out" name="ver"/>
        </method>
        <method name="Details">
            <arg type="u" direction="in" name="winid" />
            <arg type="(iiuussbu)" direction="out" name="info" />
        </method>
    </interface>
</node>`;
// x y w h title class visible id

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

    Version() {
        if (global.display.get_context().get_compositor_type() === Meta.CompositorType.WAYLAND) {
            return "2.W";
        } else {
            return "2.X";
        }
    }

    List() {
        const win = global.get_window_actors();
        const winArr = [];
        const activeWorkspace = global.workspace_manager.get_active_workspace();
        for (const w of win) {
            const rect = w.meta_window.get_frame_rect();
            let title = w.meta_window.get_title();
            let wmclass = w.meta_window.get_wm_class();
            let visible = true;
            const in_current_workspace =
                w.meta_window.located_on_workspace(activeWorkspace);
            const minimized = w.meta_window.minimized;
            if (!in_current_workspace || minimized) {
                visible = false;
            }
            if (!title) {
                title = '';
            }
            if (!wmclass) {
                wmclass = '';
            }
            const winInfo = [
                rect.x,
                rect.y,
                rect.width,
                rect.height,
                title,
                wmclass,
                visible,
                w.meta_window.get_id(),
            ];
            winArr.push(winInfo);
        }
        return winArr;
    }

    Details(winid: number) {
        const win = this._get_window_by_wid(winid);
        const activeWorkspace = global.workspace_manager.get_active_workspace();
        if (win) {
            const rect = win.meta_window.get_frame_rect();
            let title = win.meta_window.get_title();
            let wmclass = win.meta_window.get_wm_class();
            let visible = true;
            const in_current_workspace =
                win.meta_window.located_on_workspace(activeWorkspace);
            const minimized = win.meta_window.minimized;
            if (!in_current_workspace || minimized) {
                visible = false;
            }
            if (!title) {
                title = '';
            }
            if (!wmclass) {
                wmclass = '';
            }
            return [
                rect.x,
                rect.y,
                rect.width,
                rect.height,
                title,
                wmclass,
                visible,
                win.meta_window.get_id(),
            ];
        } else {
            console.debug('Not found');
            return [0, 0, 0, 0, '', '', false, 0];
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
        const win = global.get_display().get_focus_window();
        // eslint-disable-next-line @typescript-eslint/no-unnecessary-condition
        if (win) {
            return win.get_id() === winid;
        } else {
            return false;
        }
    }
}
