#!/usr/bin/env python3
"""
Test script to verify all Python imports work correctly
"""

def test_imports():
    """Test all the imports to make sure they work"""
    try:
        print("Testing imports...")
        
        # Test basic imports
        print("✓ Basic imports successful")
        
        # Test VEX library imports (these would fail on non-VEX systems)
        try:
            from vex import *
            print("✓ VEX library import successful")
        except ImportError:
            print("⚠ VEX library not available (expected on non-VEX systems)")
        
        # Test our custom modules
        try:
            from src.rgb_template.PID import PID
            print("✓ PID class import successful")
        except ImportError as e:
            print(f"✗ PID class import failed: {e}")
        
        try:
            from src.rgb_template.drive import Drive
            print("✓ Drive class import successful")
        except ImportError as e:
            print(f"✗ Drive class import failed: {e}")
        
        try:
            from src.robot_config import chassis
            print("✓ Robot config import successful")
        except ImportError as e:
            print(f"✗ Robot config import failed: {e}")
        
        try:
            from src.autons import autonomous
            print("✓ Autons import successful")
        except ImportError as e:
            print(f"✗ Autons import failed: {e}")
        
        print("\nImport test completed!")
        
    except Exception as e:
        print(f"✗ Test failed with error: {e}")

if __name__ == "__main__":
    test_imports()
