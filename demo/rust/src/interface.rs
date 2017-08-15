/* generated by rust_qt_binding_generator */
#![allow(unknown_lints)]
#![allow(mutex_atomic, needless_pass_by_value)]
#![allow(unused_imports)]
use libc::{c_int, c_uint, c_ulonglong, c_void};
use types::*;
use std::sync::{Arc, Mutex};
use std::ptr::null;

use implementation::*;

pub struct TreeQObject {}

#[derive (Clone)]
pub struct TreeEmitter {
    qobject: Arc<Mutex<*const TreeQObject>>,
    path_changed: fn(*const TreeQObject),
    new_data_ready: fn(*const TreeQObject, row: c_int, parent: usize),
}

unsafe impl Send for TreeEmitter {}

impl TreeEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn path_changed(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.path_changed)(ptr);
        }
    }
    pub fn new_data_ready(&self, row: c_int, parent: usize) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.new_data_ready)(ptr, row, parent);
        }
    }
}

pub struct TreeUniformTree {
    qobject: *const TreeQObject,
    begin_reset_model: fn(*const TreeQObject),
    end_reset_model: fn(*const TreeQObject),
    begin_insert_rows: fn(*const TreeQObject,row: c_int, parent: usize, c_int, c_int),
    end_insert_rows: fn(*const TreeQObject),
    begin_remove_rows: fn(*const TreeQObject,row: c_int, parent: usize, c_int, c_int),
    end_remove_rows: fn(*const TreeQObject),
}

impl TreeUniformTree {
    pub fn begin_reset_model(&self) {
        (self.begin_reset_model)(self.qobject);
    }
    pub fn end_reset_model(&self) {
        (self.end_reset_model)(self.qobject);
    }
    pub fn begin_insert_rows(&self,row: c_int, parent: usize, first: c_int, last: c_int) {
        (self.begin_insert_rows)(self.qobject,row, parent, first, last);
    }
    pub fn end_insert_rows(&self) {
        (self.end_insert_rows)(self.qobject);
    }
    pub fn begin_remove_rows(&self,row: c_int, parent: usize, first: c_int, last: c_int) {
        (self.begin_remove_rows)(self.qobject,row, parent, first, last);
    }
    pub fn end_remove_rows(&self) {
        (self.end_remove_rows)(self.qobject);
    }
}

pub trait TreeTrait {
    fn create(emit: TreeEmitter, model: TreeUniformTree) -> Self;
    fn emit(&self) -> &TreeEmitter;
    fn get_path(&self) -> String;
    fn set_path(&mut self, value: String);
    fn row_count(&self, row: c_int, parent: usize) -> c_int;
    fn can_fetch_more(&self, c_int, usize) -> bool { false }
    fn fetch_more(&mut self, c_int, usize) {}
    fn file_name(&self, row: c_int, parent: usize) -> String;
    fn file_icon(&self, row: c_int, parent: usize) -> Vec<u8>;
    fn file_path(&self, row: c_int, parent: usize) -> String;
    fn file_permissions(&self, row: c_int, parent: usize) -> c_int;
    fn file_type(&self, row: c_int, parent: usize) -> c_int;
    fn file_size(&self, row: c_int, parent: usize) -> c_ulonglong;
    fn index(&self, row: c_int, parent: usize) -> usize;
    fn parent(&self, parent: usize) -> QModelIndex;
}

#[no_mangle]
pub extern "C" fn tree_new(qobject: *const TreeQObject,
        path_changed: fn(*const TreeQObject),
        new_data_ready: fn(*const TreeQObject, row: c_int, parent: usize),
        begin_reset_model: fn(*const TreeQObject),
        end_reset_model: fn(*const TreeQObject),
        begin_insert_rows: fn(*const TreeQObject,row: c_int, parent: usize,
            c_int,
            c_int),
        end_insert_rows: fn(*const TreeQObject),
        begin_remove_rows: fn(*const TreeQObject,row: c_int, parent: usize,
            c_int,
            c_int),
        end_remove_rows: fn(*const TreeQObject))
        -> *mut Tree {
    let emit = TreeEmitter {
        qobject: Arc::new(Mutex::new(qobject)),
        path_changed: path_changed,
        new_data_ready: new_data_ready,
    };
    let model = TreeUniformTree {
        qobject: qobject,
        begin_reset_model: begin_reset_model,
        end_reset_model: end_reset_model,
        begin_insert_rows: begin_insert_rows,
        end_insert_rows: end_insert_rows,
        begin_remove_rows: begin_remove_rows,
        end_remove_rows: end_remove_rows,
    };
    let d = Tree::create(emit, model);
    Box::into_raw(Box::new(d))
}

#[no_mangle]
pub unsafe extern "C" fn tree_free(ptr: *mut Tree) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn tree_path_get(ptr: *const Tree,
        p: *mut c_void,
        set: fn(*mut c_void, QString)) {
    let data = (&*ptr).get_path();
    set(p, QString::from(&data));
}

#[no_mangle]
pub unsafe extern "C" fn tree_path_set(ptr: *mut Tree, v: QStringIn) {
    (&mut *ptr).set_path(v.convert());
}

#[no_mangle]
pub unsafe extern "C" fn tree_row_count(ptr: *const Tree, row: c_int, parent: usize) -> c_int {
    (&*ptr).row_count(row, parent)
}
#[no_mangle]
pub unsafe extern "C" fn tree_can_fetch_more(ptr: *const Tree, row: c_int, parent: usize) -> bool {
    (&*ptr).can_fetch_more(row, parent)
}
#[no_mangle]
pub unsafe extern "C" fn tree_fetch_more(ptr: *mut Tree, row: c_int, parent: usize) {
    (&mut *ptr).fetch_more(row, parent)
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_name(ptr: *const Tree,
                                    row: c_int, parent: usize,
        d: *mut c_void,
        set: fn(*mut c_void, QString)) {
    let data = (&*ptr).file_name(row, parent);
    set(d, QString::from(&data));
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_icon(ptr: *const Tree,
                                    row: c_int, parent: usize,
        d: *mut c_void,
        set: fn(*mut c_void, QByteArray)) {
    let data = (&*ptr).file_icon(row, parent);
    set(d, QByteArray::from(&data));
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_path(ptr: *const Tree,
                                    row: c_int, parent: usize,
        d: *mut c_void,
        set: fn(*mut c_void, QString)) {
    let data = (&*ptr).file_path(row, parent);
    set(d, QString::from(&data));
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_permissions(ptr: *const Tree, row: c_int, parent: usize) -> c_int {
    (&*ptr).file_permissions(row, parent)
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_type(ptr: *const Tree, row: c_int, parent: usize) -> c_int {
    (&*ptr).file_type(row, parent)
}

#[no_mangle]
pub unsafe extern "C" fn tree_data_file_size(ptr: *const Tree, row: c_int, parent: usize) -> c_ulonglong {
    (&*ptr).file_size(row, parent)
}

#[no_mangle]
pub unsafe extern "C" fn tree_index(ptr: *const Tree, row: c_int, parent: usize) -> usize {
    (&*ptr).index(row, parent)
}
#[no_mangle]
pub unsafe extern "C" fn tree_parent(ptr: *const Tree, parent: usize) -> QModelIndex {
    (&*ptr).parent(parent)
}
