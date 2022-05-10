//
//  StatefulPreviewWrapper.swift
//  Patriot
//
//  This is used for previewing a view with a binding
//  instead of creating a 2nd class just for previewing
//
//  See example in MainView preview
//
//  Created by Ron Lisle on 7/13/21.
//

import SwiftUI

struct StatefulPreviewWrapper<Value, Content: View>: View {
    @State var value: Value
    var content: (Binding<Value>) -> Content

    var body: some View {
        content($value)
    }

    init(_ value: Value, content: @escaping (Binding<Value>) -> Content) {
        self._value = State(wrappedValue: value)
        self.content = content
    }
}
