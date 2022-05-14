//
//  EntryTextField.swift
//  Patriot
//
//  Created by Ron Lisle on 5/13/22.
//  Based on RW Focus tutorial Given In Love example
//

import SwiftUI

struct EntryTextField: View {
  var sfSymbolName: String
  var placeHolder: String
  var prompt: String
  @Binding var field: String
  var isSecure = false

  var body: some View {
    VStack(alignment: .leading) {
      HStack {
        Image(systemName: sfSymbolName)
          .foregroundColor(.gray)
          .font(.headline)
        if isSecure {
          SecureField(placeHolder, text: $field).autocapitalization(.none)
        } else {
          TextField(placeHolder, text: $field).autocapitalization(.none)
        }
      }
      .padding(8)
      .background(Color(UIColor.systemBackground))
      .overlay(RoundedRectangle(cornerRadius: 8).stroke(prompt.isEmpty ? Color.gray : Color.red, lineWidth: 1))
      Text(prompt)
        .fixedSize(horizontal: false, vertical: true)
        .font(.caption)
    }
  }
}
