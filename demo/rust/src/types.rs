/* generated by rust_qt_binding_generator */
#![allow(dead_code)]
use std::slice;
use libc::{c_int, uint8_t, uint16_t};

#[repr(C)]
pub struct COption<T> {
    data: T,
    some: bool,
}

impl<T> From<Option<T>> for COption<T> where T: Default {
    fn from(t: Option<T>) -> COption <T> {
        if let Some(v) = t {
            COption {
                data: v,
                some: true
            }
        } else {
            COption {
                data: T::default(),
                some: false
            }
        }
    }
}

#[repr(C)]
pub struct QString {
    data: *const uint8_t,
    len: c_int,
}

#[repr(C)]
pub struct QStringIn {
    data: *const uint16_t,
    len: c_int,
}

impl QStringIn {
    pub fn convert(&self) -> String {
        let data = unsafe { slice::from_raw_parts(self.data, self.len as usize) };
        String::from_utf16_lossy(data)
    }
}

impl<'a> From<&'a String> for QString {
    fn from(string: &'a String) -> QString {
        QString {
            len: string.len() as c_int,
            data: string.as_ptr(),
        }
    }
}

#[repr(C)]
pub struct QByteArray {
    data: *const uint8_t,
    len: c_int,
}

impl QByteArray {
    pub fn convert(&self) -> Vec<u8> {
        let data = unsafe { slice::from_raw_parts(self.data, self.len as usize) };
        Vec::from(data)
    }
}

impl<'a> From<&'a Vec<u8>> for QByteArray {
    fn from(value: &'a Vec<u8>) -> QByteArray {
        QByteArray {
            len: value.len() as c_int,
            data: value.as_ptr(),
        }
    }
}

#[repr(C)]
pub struct QModelIndex {
    row: c_int,
    internal_id: usize,
}

impl QModelIndex {
    pub fn invalid() -> QModelIndex {
        QModelIndex {
            row: -1,
            internal_id: 0,
        }
    }
    pub fn create(row: c_int, id: usize) -> QModelIndex {
        QModelIndex {
            row: row,
            internal_id: id,
        }
    }
}

#[repr(C)]
pub enum SortOrder {
    Ascending = 0,
    Descending = 1
}

