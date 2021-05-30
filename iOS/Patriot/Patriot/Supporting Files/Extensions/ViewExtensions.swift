//
//  ViewExtensions.swift
//  Patriot
//
//  Created by Ron Lisle on 5/27/21.
//

import SwiftUI

extension View {
    @ViewBuilder func isHidden(_ isHidden: Bool) -> some View {
        if isHidden {
            self.hidden()
        } else {
            self
        }
    }
}

