//
//  PartOfDayButton.swift
//  Patriot
//
//  Created by Ron Lisle on 5/11/22.
//

import SwiftUI

struct PartOfDayButton: View {
    
    @EnvironmentObject var model: PatriotModel

    var body: some View {
        Button(action: {
            withAnimation {
                print("PartOfDay")
            }
        }) {
            PartOfDayView()
        }.foregroundColor(Color("TextColor"))
    }
}

struct PartOfDayView: View {

    @EnvironmentObject var model: PatriotModel
    
    var body: some View {
        Image(systemName: partOfDayIcon())
            .imageScale(.large)
    }
    
    func partOfDayIcon() -> String {
        switch model.partOfDay {
        case .Night:
            return "moon.stars.fill"
        case .Dawn, .Dusk:
            return "moon.fill"
        case .Sunrise, .Sunset:
            return "sun.and.horizon"
        case .Noon:
            return "sun.max"
        case .unknown:
            return "questionmark"
        default:
            return "sun.min"
        }
    }
}

struct PartOfDayButton_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Night))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Night")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Sunrise))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Sunrise")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Morning))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Morning")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Noon))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Noon")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Afternoon))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Afternoon")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Sunset))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Sunset")
                .background(Color("BackgroundColor"))

            PartOfDayButton()
                .environmentObject(PatriotModel(forTest: true, partOfDay: .Dusk))
                .previewLayout(.sizeThatFits)
                .padding()
                .previewDisplayName("Dusk")
                .background(Color("BackgroundColor"))
        }
    }
}
